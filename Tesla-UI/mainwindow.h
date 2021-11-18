#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void on_previewButton_clicked();
    void on_pushButton_7_clicked();
    void on_actionExit_triggered();
    void on_actionOptions_triggered();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_ZoomY_valueChanged(double );
    void on_ZoomX_valueChanged(double );
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();

public slots:

};





#endif // MAINWINDOW_H


