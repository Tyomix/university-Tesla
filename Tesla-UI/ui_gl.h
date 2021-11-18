/********************************************************************************
** Form generated from reading UI file 'gl.ui'
**
** Created: Wed Apr 17 01:03:32 2013
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GL_H
#define UI_GL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gl
{
public:

    void setupUi(QWidget *gl)
    {
        if (gl->objectName().isEmpty())
            gl->setObjectName(QString::fromUtf8("gl"));
        gl->resize(400, 300);

        retranslateUi(gl);

        QMetaObject::connectSlotsByName(gl);
    } // setupUi

    void retranslateUi(QWidget *gl)
    {
        gl->setWindowTitle(QApplication::translate("gl", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class gl: public Ui_gl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GL_H
