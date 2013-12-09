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

#include <QtGui>
#include <qpgui.h>

#include "layout.h"
#include "mtnamespace.h"
//#include "style.h"
#include "defaultdlg.h"

using namespace mt;

DefaultDlg::DefaultDlg(QWidget *parent) :
    QDialog(parent)
{
    QpAppStyle style;
    setStyleSheet(*style.getAppStyle());
    setWindowTitle(QString("Set Default Values"));
    init();
}

void DefaultDlg::onOk()
{
    this->done(0);
}

void DefaultDlg::init()
{
    int index;

    // Set the starting coordinates in pixels for the objects that will be
    // laid out.
    //
    const int tests_x = 40;     // x coord of left side of groups
    const int tests_y = 50;     // y coord of top of groups
    const int text_w = 80;      // Width of all text
    const int text_h = 14;      // Height of all text
    const int box_w = 60;       // Width of Edit Boxes and Labels
    const int box_h = 30;       // Height of all controls
    const int vs = 36;          // Vertical Span
    const int hs = 36;          // Horizontal span

    // Instantiate the Layout parameters.
    // See layout.h
    //
    LAYOUT_INIT()

    // Layout the Tests Checkboxes
    //
    LAYOUT(tests_x, tests_y, 0, vs)
    Twidget *tw = new Twidget;
    tw->objName = "Tests";
    tw->objText << "Add" << "Subtract" << "Multiply" << "Divide";
    tw->sizes   << QSize(text_w, box_h);
    tw->layout  << CELL(0,0) << CELL(0,1) << CELL(0,2) << CELL(0,3);
    tw->connect = false;
    //
    // Layout the Labels for number of Problems, time in Seconds, and Grade
    // Level.
    // These labels will "belong" to the pTests control group.
    //
    int label_x  = tests_x + text_w + hs;
    int label_y  = 14;
    int label_hs = box_w + hs;
    LAYOUT(label_x, label_y, label_hs, text_h)
    tw->labelText   << "Number of" << "Problems"
                    << "Time in" << "Seconds"
                    << "Grade" << "Level";
    tw->labelSizes  << QSize(text_w, text_h);
    tw->labelLayout << CELL(0,0) << CELL(0,1)
                    << CELL(1,0) << CELL(1,1)
                    << CELL(2,0) << CELL(2,1);
    pTests = new TButtonGroup <QCheckBox>(tw, this);

    // Layout the "Problems" QLineEdit boxes
    //
    tw = new Twidget;
    int prob_x = label_x;
    int prob_y = tests_y;
    LAYOUT(prob_x, prob_y, 0, vs)
    tw->objName = "Problems";
    tw->objText << "5" << "5" << "5" << "5";
    tw->sizes   << QSize(box_w, box_h);
    tw->layout  << CELL(0,0) << CELL(0,1) << CELL(0,2) << CELL (0,3);
    tw->connect = true;
    pProblems   = new TEditGroup <QLineEdit>(tw, this);
    for(index = 0; index < pProblems->widgetList.size(); index++)
        pProblems->widgetList[index]->setInputMask("00");

    // Layout the "Seconds" QLineEdit boxes
    //
    tw = new Twidget;
    int time_x = label_x + box_w + hs;
    int time_y = prob_y;
    LAYOUT(time_x, time_y, 0, vs);
    tw->layout  << CELL(0,0) << CELL(0,1) << CELL(0,2) << CELL(0,3);
    tw->objName = "Seconds";
    tw->objText << "10" << "10" << "25" << "30";
    tw->sizes   << QSize(box_w, box_h);
    tw->connect = false;
    pTimes      = new TEditGroup <QLineEdit>(tw, this);
    for(index = 0; index < pTimes->widgetList.size(); index++)
        pTimes->widgetList[index]->setInputMask("00");

    // Layout the Grade Level radiobuttons.
    //
    tw = new Twidget;
    int gl_x = time_x + box_w + hs;
    int gl_y = time_y;
    LAYOUT(gl_x, gl_y, 0, vs);
    tw->layout << CELL(0,0) << CELL(0,1) << CELL(0,2) << CELL(0,3);
    tw->sizes << QSize(box_w - 10, box_h);
    tw->objText << "1" << "2" << "3" <<"4";
    tw->connect = true;
    tw->grouped = true;
    pGradeLevel = new TButtonGroup <QRadioButton>(tw, this);

    // Layout the UserName box and its label
    //
    int userName_x = tests_x;
    int userName_y = tests_y + text_h + (4 * vs);
    int userNameEd_x = userName_x + text_w;
    int userNameEd_w = 210;
    userNameLabel = new QLabel(this);
    userNameLabel->setGeometry(userName_x, userName_y, text_w, text_h);
    userNameLabel->setText("Your Name:");
    userNameEdit = new QLineEdit(this);
    userNameEdit->setGeometry(userNameEd_x, userName_y, userNameEd_w, box_h);
    userNameEdit->setText("");
    userNameEdit->setCursorPosition(0);

    const int pbw = 90;
    int okButton_x = tests_x+130;
    int okButton_y = userName_y + vs;
    okButton = new QPushButton("Done", this);
    okButton->setGeometry(okButton_x, okButton_y, pbw, box_h);
    okButton->setAutoDefault(false);
    okButton->setDefault(false);
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOk()));
}
