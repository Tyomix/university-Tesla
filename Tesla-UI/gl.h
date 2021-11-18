#ifndef GL_H
#define GL_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>




namespace Ui {
  class gl;
}

class gl: public QGLWidget
{



    Q_OBJECT

public:
    gl(QWidget *parent = 0);
    QLabel* pLabel;
    QTimer *Timer;


protected:


    bool singling; // Для выделение области
    void self_cursor(); // метод для рисования своего курсора
    void initializeGL(); // Метод для инициализирования opengl
    void resizeGL(int nWidth, int nHeight); // Метод вызываемый после каждого изменения размера окна
    void paintGL(); // Метод для вывода изображения на экран
    void keyPressEvent(QKeyEvent *ke); // Для перехвата нажатия клавиш на клавиатуре
    void mousePressEvent(QMouseEvent *me); // Реагирует на нажатие кнопок мыши


public slots:
     void image();
     void autoimage();


};

#endif // GL_H
