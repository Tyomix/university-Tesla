# Fast ADC device with STM32F407VGT6 ARM microcontroller 
[РУССКАЯ ВЕРСИЯ НИЖЕ](https://github.com/Tyomix/university-Tesla#Использование-ARM-микроконтроллера-STM32F407VGT6-для-построения-быстродействующего-АЦП-с-выводом-результатов-на-ЭВМ)

The purpose of this project is the development of an analog-to-digital converter (ADC) with PC interface for digitizing a video signal of the scanning electron microscope (SEM) Tesla BS 300.  The microscope was designed to render image using analog CRT monitor with resolution up to 1600 lines height and with a single line scan time from 1 to 500 ms. With the advancement of information technology it became necessary to save SEM images directly in computer files. Therefore it was decided to develop an analog-to-digital converter to digitize this analog video signal. It was estimated that ADC should be able to work with at least 100 kHz sampling rate to provide reasonable quality images with vertical resolution of 1600 pixels at 50 ms line scan time. The resolution of ADC samples must be at least 8 bit to provide 256 different gray color levels which is sufficient for a regular computer monitor.

There are many different ways of creating such kind of a device but it was proposed to use STM32F4DISCOVERY development board based on a STM32F407VGT6 microcontroller with has 168 MHz ARM Cortex-M4F core, 1024 KB Flash, 192 KB RAM, three 12-bit ADC with frequency up to 7.2 MSPS, USB device and USB host interfaces and other peripherals. STM32F4DISCOVERY board also includes an integrated ST-LINK/V2 debugger interface, which allows programming and debugging the microcontroller. One of the advantages of the development board in comparison with a single microcontroller is that the development board has a convenient pinout. Thus there is no necessity to solder a tiny SMD chip each time during the experiments with different circuits. We decided to use USB connection in serial port emulation mode (USB-CDC) to connect the microcontroller with a computer, because this interface is well-documented and provides sufficient data transferring rate for our purpose. To accomplish this project we wrote the firmware for microcontroller in C programming language and the software the computer in C++.

Logic of the firmware is described below. ADC of the microcontroller constantly makes conversions at three channels with 200 kHz frequency and 8 bit resolution. The first channel is luminance signal. Two other channels are horizontal and vertical synchronization signals. After making each conversion direct memory access controller (DMA) copies the result from the ADC register into a random access memory (RAM) buffer of the microcontroller. The data of digitized luminance signal starts to be sent via USB-CDC after filling the first half of the buffer. Each result is sent as a byte. The synchronization data are processed by a program algorithm in order to recognize synchronization pulses. As far as ad end of a line or an end of a frame is detected, one or two zero bytes are sending into the port, respectively. At the same time DMA keeps on filling the buffer independently from CPU. After filling the second half of the buffer the data of digitized luminance signal are sent out in the same way. DMA starts to fill the buffer from the outset. Using DMA and RAM buffer allows us to reduce CPU loading and significantly increase a sampling rate. 

A standard x86 PC which runs Debian GNU/Linux operation system was used as a data processing platform. The image processing software was created on Qt 4. Qt is a cross-platform application framework for developers who use C++. Figure 1 shows a user interface (UI) of the data processing software. UI consists of a control window and a window, which is used to render the image of an investigated specimen. This window can be turned to a full screen by double click or hotkey. 

![](https://github.com/Tyomix/university-Tesla/blob/main/1.png?raw=true)

_Figure 1. User interface of the software._

After a user starts working by clicking the “Preview” button the program begins to read data from a virtual serial port through which our microcontroller is connected. Data collection task runs in a separate high priority thread. These data are placed into an array in physical memory. As far as a whole frame is obtained the image is rendered using OpenGL. The linear interpolation is applied to the image in order to get a right aspect ratio. Scale bar, current time and date are rendered as well beneath a micrograph. The image is refreshed after receiving a new frame. A real time previewing of an image from SEM is carried out in that way. There is an opportunity to save a micrograph as PNG image file. PNG format provides good lossless image compression. You can see a micrograph example on figure 2.

![](https://github.com/Tyomix/university-Tesla/blob/main/2.png?raw=true)

_Figure 2. A micrograph of Scots pine (Pinus sylvestris L.) pollen grains._



# Использование ARM микроконтроллера STM32F407VGT6 для построения быстродействующего АЦП с выводом результатов на ЭВМ

Целью данного проекта являлась разработка аналого-цифрового преобразователя с выводом результатов на ЭВМ для электронного микроскопа Tesla BS 300.
Для реализации проекта была выбрана отладочная плата на базе микроконтроллера архитектуры ARM серии STM32F4 от компании ST Microelectronics – STM32F4DISCOVERY поскольку она сочетает в себе высокое быстродействие и богатый набор периферии: тактовая частота ядра 168 МГц, 
1 МБ Flash, 192 КБ RAM, три 12-и битных АЦП с частотой до 7.2 MSPS, ЦАП, I2C, USART, CAN, SPI, LIN, IrDA, I2S, SDIO, USB device/host/OTG, Ethernet, модуль CRC. Передача данных измерений на ПК осуществляется через USB микроконтроллера в режиме эмуляции COM порта (USB-CDC). 
В роли платформы для обработки данных и построения изображения, полученного с электронного микроскопа в проекте используется компьютер работающий под управлением Debian GNU/Linux. Для разработки программы  был использован инструментарий Qt 4. Интерфейс программы обработки данных состоит из основного окна и окна, используемого для отрисовки изображения. На основе полученных от микроконтроллера данных строится изображение при помощи графической библиотеки OpenGL. Масштаб полученного изображения можно изменять независимо как по вертикали, так и по горизонтали.
В качестве эксперимента с помощью микроскопа при увеличении около 400 раз было получено несколько изображений поверхности кристалла микросхемы при различных разрешениях и временах развертки. Наилучших результатов удалось добиться при разрешении 800 строк с временем развертки 200 мс/строку. 

![](https://github.com/Tyomix/university-Tesla/blob/main/3.png?raw=true)

_Рис. 3. Изображение полученное при разрешении 400 строк с временем развертки 5 мс/строку._

![](https://github.com/Tyomix/university-Tesla/blob/main/4.png?raw=true)

_Рис. 4. Изображение полученное при разрешении 800 строк с временем развертки 200 мс/строку._
