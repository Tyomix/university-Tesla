#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfile.h>
#include <qtextstream.h>
#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include "gl.h"
#include "main.h"
#include <QtConcurrentRun>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->previewButton->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_7->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButton_5->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_6->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogCloseButton));
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::on_previewButton_clicked()
{
        work=true;
        //if (QFile::exists(devname))
        {
            QtConcurrent::run(ThreadADC);
        }
        //else
       // {
       //    QMessageBox::critical(this, "Error", "Device "+devname+" not found!");
      //  }

}

void MainWindow::on_pushButton_2_clicked()
{

    for (long i=0;i<(4000*1800);i++)
       {
        data1[i]=i;
        }
}

void MainWindow::on_pushButton_3_clicked()
{

    for (long i=0;i<(4000*1800);i++)
       {
        data1[i]=0;
        }
}

void MainWindow::on_ZoomX_valueChanged(double x)
{
    zoomX=x;
}

void MainWindow::on_ZoomY_valueChanged(double y)
{
    zoomY=y;
}


void MainWindow::on_pushButton_4_clicked()
{
    if (done)
    {
    qDebug() << "Done";
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    work=false;
}

void MainWindow::on_pushButton_6_clicked()
{
    work=false;
    exit(0);
}

void MainWindow::on_actionOptions_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    work=false;
    exit(0);
}

void MainWindow::on_pushButton_7_clicked()
{

}
