 /*
Copyright (2012-2013) Artyom
Distributed under the GNU GPLv3.
*/

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include <stdio.h>
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"
#include "misc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TESTRESULT_ADDRESS         0x080FFFFC
#define ALLTEST_PASS               0x00000000
#define ALLTEST_FAIL               0x55555555
//////////////////////////////////////////////////////////////////////////////////////////
#define N_WAVE  512*3    /* размер буф. БПФ*/
#define Fsimpl  200000 //частота оцифровки (Гц)


u16 ADCConvertedValue[N_WAVE*2];

int a=0;
int Vsunc0=0;
int Hsunc0=0;
int Vth=40; //чувствительность определения конца кадра (меньшее значение соответствует большей чувствительности)
int Hth=40; //чувствительность определения конца строки (меньшее значение соответствует большей чувствительности)

volatile int buff_adc_flag;

// адрес регистра данных, откуда будем брать результат конвертации
#define ADC_CDR_ADDRESS    ((uint32_t)0x40012308)


// переменные, описывающие свойства периферии
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
DMA_InitTypeDef DMA_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
//////////////////////////////////////////////////////////////////////////////////////////

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
  
uint16_t PrescalerValue = 0;

__IO uint32_t TimingDelay;
__IO uint8_t DemoEnterCondition = 0x00;
__IO uint8_t UserButtonPressed = 0x00;

uint8_t Buffer[6];


////////////////////////////////////////////////////////////////////////////////////

  //dma2 stream 0 Прерывание;
  void DMA2_Stream0_IRQHandler ( void )
  {
     // Проверка DMA Stream на заполнение всего буффера
  if ( DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) )
  {
   // Очистить флаг заполнения всего буффера
    DMA_ClearITPendingBit ( DMA2_Stream0, DMA_IT_TCIF0 );
    buff_adc_flag=1;
  }
  // Проверка DMA Stream на заполнение половины буффера
  if ( DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0) )
  {
    // Очистить флаг заполнения половины буффера
    DMA_ClearITPendingBit ( DMA2_Stream0, DMA_IT_HTIF0 );
    buff_adc_flag=2;
  }
  }
////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);  


  
  ///////////////////////////////////////////////////////
  

    // USB configuration 
      USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);




   // разрешаем тактирование используемых периферий
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);

   // описываем свойства портов ввода-вывода по которым будем производить оцифровку
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_1 | GPIO_Pin_2 ;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
     GPIO_Init(GPIOA, &GPIO_InitStructure);

   // описываем свойства DMA через который будет вестись обмен
     DMA_InitStructure.DMA_Channel = DMA_Channel_0;
     DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CDR_ADDRESS;       // адрес регистра данных АЦП
     DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCConvertedValue;
     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
     DMA_InitStructure.DMA_BufferSize = N_WAVE*2;
     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
     DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
     DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
     DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
     DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
     DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
     DMA_Init(DMA2_Stream0, &DMA_InitStructure);

      //Настраиваем прерывания DMA по заполнению половины и всего буффера
      NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      DMA_ITConfig(DMA2_Stream0, DMA_IT_HT | DMA_IT_TC, ENABLE);

   // запуск DMA
     DMA_Cmd(DMA2_Stream0, ENABLE);

    // отключаем АЦП
     ADC_Cmd(ADC1, DISABLE);
     ADC_Cmd(ADC2, DISABLE);
     ADC_Cmd(ADC3, DISABLE);

   // описываем АЦП
   // общие параметры работы АЦП
     ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;
     ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_6Cycles;
     ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
     ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
     ADC_CommonInit(&ADC_CommonInitStructure);

   // настраиваем АЦП1
     ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
     ADC_InitStructure.ADC_ScanConvMode = DISABLE;
     ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
     ADC_InitStructure.ADC_ExternalTrigConvEdge =  ADC_ExternalTrigConvEdge_Rising;
     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_TRGO;
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
     ADC_InitStructure.ADC_NbrOfConversion = 1;
     ADC_Init(ADC1, &ADC_InitStructure);
   // указывем канал, подлежащий оцифровке
     ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);

     // настраиваем АЦП2
     ADC_Init(ADC2, &ADC_InitStructure);
   // указывем канал, подлежащий оцифровке
     ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_15Cycles);
     
      // настраиваем АЦП3
     ADC_Init(ADC3, &ADC_InitStructure);
   // указывем канал, подлежащий оцифровке
     ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_15Cycles);
     
   // разрешаем выполнять запросы DMA

     ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
   // запускаем АЦП
     ADC_Cmd(ADC1, ENABLE);      
     ADC_Cmd(ADC2, ENABLE);
     ADC_Cmd(ADC3, ENABLE);
     
   // настраиваем таймер
   TIM_Cmd(TIM8, DISABLE);
   // подключаем тактирование
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
   // описываем таймер
     TIM_TimeBaseStructure.TIM_Period = (168000000/Fsimpl);
     TIM_TimeBaseStructure.TIM_Prescaler = 0;
     TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
     TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
   // записываем описание в регистры
     TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
   // сбрасываем, указываем выходной триггер (для запуска АЦП)
     TIM_SetCounter(TIM8, 0);
     TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);
   // запукаем таймер
     TIM_Cmd(TIM8, ENABLE);

     buff_adc_flag=0;
     
     Vsunc0=(ADCConvertedValue[2]);
     Hsunc0=(ADCConvertedValue[1]);
     
     int b=1;
      
    
  while (1) //основной цикл
   {
          while(buff_adc_flag==0);
        if (buff_adc_flag==2)
     {
        buff_adc_flag=0;
        if ((Vsunc0-ADCConvertedValue[2])>Vth)
          {
            putchar(0x0);
            putchar(0x0); 
          }
        Vsunc0=ADCConvertedValue[2];
        
         
          for(a=0;a<N_WAVE;a+=3)
            {
              if ((Hsunc0-ADCConvertedValue[a+1])<Hth)        
              {
               if (ADCConvertedValue[a]) putchar(ADCConvertedValue[a]);
               else putchar(0x1);
              }
              else
              {
                 if (b>0)
                 {
                  putchar(0x0);
                 b=-5;
                 }                
              }
            Hsunc0=ADCConvertedValue[a+1];
            b++;
          
            }
          
            
         
      }

     
     if (buff_adc_flag==1)
     {
        buff_adc_flag=0;
        
        if ((Vsunc0-ADCConvertedValue[N_WAVE+2])>Vth)
          {
            putchar(0x0);
            putchar(0x0); 
          }
        Vsunc0=ADCConvertedValue[N_WAVE+2];
               
         for(a=N_WAVE;a<N_WAVE*2;a+=3)
       {
         
          
              if ((Hsunc0-ADCConvertedValue[a+1])<Hth)        
              {
               if (ADCConvertedValue[a]) putchar(ADCConvertedValue[a]);
               else putchar(0x1);
              }
              else
              {
                 if (b>0)
                 {
                  putchar(0x0); 
                 b=-5;
                 }                
              }
              
              b++;
              Hsunc0=ADCConvertedValue[a+1];
       }
     }
     

  }

  
  ///////////////////////////////////////////////////////
  

}



/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

