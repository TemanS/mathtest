/********************************************************************************
** Form generated from reading UI file 'mathtest.ui'
**
** Created: Mon Nov 21 08:30:00 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATHTEST_H
#define UI_MATHTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MathTest
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QLineEdit *lineEdit_2;
    QCheckBox *checkBox_3;
    QLineEdit *lineEdit_3;

    void setupUi(QDialog *MathTest)
    {
        if (MathTest->objectName().isEmpty())
            MathTest->setObjectName(QString::fromUtf8("MathTest"));
        MathTest->resize(400, 300);
        gridLayoutWidget = new QWidget(MathTest);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(110, 110, 204, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(gridLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 2, 1, 1);

        checkBox_2 = new QCheckBox(gridLayoutWidget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 1, 0, 1, 1);

        checkBox = new QCheckBox(gridLayoutWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 0, 0, 1, 1);

        lineEdit_2 = new QLineEdit(gridLayoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 0, 2, 1, 1);

        checkBox_3 = new QCheckBox(MathTest);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(30, 30, 70, 17));
        lineEdit_3 = new QLineEdit(MathTest);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(120, 30, 113, 20));

        retranslateUi(MathTest);

        QMetaObject::connectSlotsByName(MathTest);
    } // setupUi

    void retranslateUi(QDialog *MathTest)
    {
        MathTest->setWindowTitle(QApplication::translate("MathTest", "MathTest", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("MathTest", "CheckBox", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MathTest", "CheckBox", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("MathTest", "CheckBox", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MathTest: public Ui_MathTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATHTEST_H
