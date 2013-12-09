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

#ifndef DEFAULTDLG_H
#define DEFAULTDLG_H

#include <QtCore>
#include <QtGui>
#include <QCheckBox>
#include <QObject>
#include <QList>
#include <QDialog>
#include <QFile>
#include <QLineEdit>
#include <QPushButton>

#include "controlgroup.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QCheckBox;
class QLineEdit;
class QString;
QT_END_NAMESPACE

class DefaultDlg : public QDialog
{
    Q_OBJECT

public:

    /******************************************************************
    ** Functions
    *****************************************************************/
    explicit DefaultDlg (QWidget *parent);

    /******************************************************************
    ** Data
    *****************************************************************/
    TButtonGroup <QCheckBox> *pTests;
    TEditGroup <QLineEdit> *pProblems;
    TEditGroup <QLineEdit> *pTimes;
    TEditGroup <QLineEdit> *pQuiz;
    TButtonGroup <QRadioButton> *pGradeLevel;
    QLineEdit *userNameEdit;
    QLabel *userNameLabel;
    QString userName;
    QPushButton *selectDefaultFile;
    QPushButton *okButton;
    QFile defaultFile;
    QString defaultFileName;
    QDialog *caller;

signals:

private slots:
    void onOk();

private:
    void init();

};

#endif // DEFAULTDLG_H
