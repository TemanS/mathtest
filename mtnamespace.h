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

#ifndef MTNAMESPACE_H
#define MTNAMESPACE_H

namespace mt
{
    enum quiz_t{qz_number, qz_quiz, qz_answer, qz_boundary};
    enum op_t {op_add, op_sub, op_mul, op_div, op_boundary};
    enum gl_t {gl_1, gl_2, gl_3, gl_4, gl_boundary};

    ///////////////////////////////////////////////////////////
    // Window Size
    ///////////////////////////////////////////////////////////
    const static int winWid = 380;
    const static int winHgt = 560;
}

#endif // MTNAMESPACE_H
