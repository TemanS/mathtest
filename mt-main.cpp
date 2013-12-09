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

#include <QApplication>
#include <QDesktopWidget>
#include "mtnamespace.h"
#include "mathtest.h"

using namespace mt;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // This code centers the app on the desktop.
    //
    QDesktopWidget *desktop = a.desktop();
    int screenWidth;
    int screenHeight;
    int x, y;

    screenWidth = desktop->width();
    screenHeight = desktop->height();

    x = (screenWidth - mt::winWid) / 2;
    y = (screenHeight - mt::winHgt) / 2;
    y -= 50;

    MathTest w(QPoint(x, y));

    // Show the app and execute it.
    //
    w.show();
    return a.exec();
}
