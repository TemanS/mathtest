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

#ifndef LAYOUT_H
#define LAYOUT_H

// Convenient Macros for placing objects in columns and rows.
// Next project, just use QLayout.
//
// Use the LAYOUT macro to declare the (x,y) starting point in pixels,
// as well as the horizontal and vertical spans in pixes to the next
// QObject.
//
// Then use the POINT macro to place each object with simple (column,row)
// notation.

// LAYOUT - takes the x and y coordinates in pixels relative to the upper left
//          corner of the window on which the widget will be placed.
//
// x    - pixel x-coordinate of upper left corner of widget relative to
//        upper left corner of the window upon which it is placed.
// y    - pixel y-coordinate of upper left corner of widget.
// hs   - horizontal span, distance in pixels to the next widget to the right
//        in the layout. Zero here indicates the layout is vertical.
// vs   - vertical span, distance in pixels to the next widget below in the
//        layout. Zero here indicates the layout is horizontal.
//
#define LAYOUT_INIT() \
    int _Lx; \
    int _Ly; \
    int _Lhs; \
    int _Lvs;

#define LAYOUT(x,y,hs,vs) \
    _Lx = x; \
    _Ly = y; \
    _Lhs = hs; \
    _Lvs = vs;

// CELL
// This macro allows us to use "column,row" placement of widgets, once the
// LAYOUT macro is called to express the layout in pixels. The CELL macro
// takes the const int definitions made by LAYOUT and converts them to
// pixel coordinates in a QPoint. All the user has to do is indicate which
// column,row for the widget, wich makes layout a lot easier.
//
#define CELL(c, r) QPoint((_Lx + (c * _Lhs)), (_Ly + (r * _Lvs)))


#endif // LAYOUT_H
