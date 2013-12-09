/******************************************************************************
**
**  mathtest - an application exercising basic arithmetic skills.
**
**  Tony Camuso
**  November, 2011
**
**  Version 0.1
**
**    mathtest is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**  GNU General Public License http://www.gnu.org/licenses/gpl.html
**
**  Copyright 2011 by Tony Camuso.
**
******************************************************************************/

#ifndef MATHTEST_UI_H
#define MATHTEST_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_MathTest
{
public:

    void setupUi(QDialog *MathTest)
    {
        if (MathTest->objectName().isEmpty())
            MathTest->setObjectName(QString::fromUtf8("MathTest"));
        MathTest->resize(360, 480);

        retranslateUi(MathTest);

        QMetaObject::connectSlotsByName(MathTest);
    } // setupUi

    void retranslateUi(QDialog *MathTest)
    {
        MathTest->setWindowTitle(
            QApplication::translate(
                "MathTest", "MathTest", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MathTest: public Ui_MathTest{};
} // namespace Ui

QT_END_NAMESPACE

#endif // MATHTEST_UI_H
