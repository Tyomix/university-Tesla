#include <QtGui/QApplication>
#include "mainwindow.h"
#include "gl.h"
#include "main.h"

unsigned char data1[1800*4000];
double zoomX=0.5;
double zoomY=1;
bool done=false;
bool work=false;
bool save=false;
QString devname="/dev/ttyACM0";


int main(int argc, char *argv[])
{



    QApplication tesla(argc, argv);
    MainWindow w;
    gl window;

    w.setGeometry(0,550,500,180);
    w.show();

    window.setWindowTitle("OpenGL Window");
        window.setGeometry(0,0,700,500);
        window.show();

    return tesla.exec();

}


void ThreadADC()

{

    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    unsigned int x=0;
    unsigned int y=0;
    bool ok;


    QFile ttyACM(devname);
    ttyACM.open(QIODevice::ReadOnly | QIODevice::Text);

            QTextStream in(&ttyACM);
            QString line = in.readLine();

         if (save)
         {
           while (!line.isEmpty())
           {
                while (!line.isEmpty())
                {
                line = in.readLine();
                }
              line = in.readLine();
           }
         }


         while (work)
         {
            while ((!line.isEmpty()) && (y<1800))
            {


                while (!line.isEmpty())
                {

                    if (x<4000)
                      {
                      data1[(y*4000+x)]=255-line.toUShort(&ok,16);
                      }
                    x++;

                    line = in.readLine();

                    if (!work)
                    {
                        goto stop;
                    }


                }

               y++;
               x=0;
               line = in.readLine();
               }
            line = in.readLine();
            stop:
            done=true;
            qDebug() << QTime::currentTime().toString() <<" Read " << y << " lines";
            if (save)
            {
                break;
            }
            x=0;
            y=0;

          }
 }
