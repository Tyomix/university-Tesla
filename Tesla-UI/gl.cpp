#include "gl.h"
#include "main.h"
static bool fullscreen=false;


gl::gl(QWidget* parent) : QGLWidget(parent)
{
    singling=false;
    setFormat(QGLFormat(QGL::DoubleBuffer)); // Двойная буферизация
    glDepthFunc(GL_LEQUAL); // Буфер глубины

}

void gl::initializeGL()
{
    qglClearColor(Qt::black); // Черный цвет фона

}

void gl::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);

}



void gl::keyPressEvent(QKeyEvent *ke)
{
    switch (ke->key())
     {

     case Qt::Key_Space:
        {

           image();            
           // swapBuffers();
            break;
        }

     case Qt::Key_Up:
      {
          zoomY+=0.1;
          image();
          //swapBuffers();
          break;
      }

     case Qt::Key_Down:
      {
          zoomY-=0.1;
          image();
          //swapBuffers();
          break;
      }

     case Qt::Key_Left:
      {
          zoomX-=0.1;
          image();
          //swapBuffers();
          break;
      }

     case Qt::Key_Right:
      {
          zoomX+=0.1;
          image();
          //swapBuffers();
          break;
      }

     case Qt::Key_F:
        {
            fullscreen = !fullscreen;
            setWindowState((fullscreen ? Qt::WindowFullScreen : Qt::WindowNoState));
            break;
        }


     }






}


void gl::mousePressEvent(QMouseEvent *me)
    {
      updateGL();

    }

void gl::image()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     QString lbl;
     lbl.append("Zoom X = ");
     lbl.append(QString::number(zoomX*100));
     lbl.append("%; Zoom Y = ");
     lbl.append(QString::number(zoomY*100));
     lbl.append("%.");


      glPixelZoom(zoomX,zoomY);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      //glRasterPos2i(10,450);
      glDrawPixels(4000,1800,GL_LUMINANCE,GL_UNSIGNED_BYTE,&data1);
      renderText(10, 10 , 0, lbl, QFont() , 2000);
      swapBuffers();
      qDebug() << QTime::currentTime().toString() << " Image rendered.";

}

void gl::autoimage()
{
    if(done)
        {
        image();
        done=false;
        }
}

void gl::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,500,500,0,1,0); // подготавливаем плоскости для матрицы
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    qglColor(Qt::white);

    Timer = new QTimer();
    connect(Timer, SIGNAL(timeout()), this, SLOT(autoimage()));
    Timer->start(100);

}

