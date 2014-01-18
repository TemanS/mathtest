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

#ifndef MATHTEST_H
#define MATHTEST_H

#include <QList>
#include <QObject>
#include <QDialog>
#include <QApplication>
#include <QElapsedTimer>
#include <QMessageBox>
#include <qpgui.h>
#include <mathpack.h>
#include "mtnamespace.h"

using namespace mt;

QT_BEGIN_NAMESPACE
class QCheckBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QTextEdit;
class QTextStream;
class QDateTime;
class QMessageBox;
QT_END_NAMESPACE

#define MAXPROB 100     // no more than 100 problems
#define MAXTIME 60      // one minute max

class MathTest : public QDialog
{
    Q_OBJECT

public:
    explicit MathTest(QPoint origin, QWidget *parent = 0);
    ~MathTest();

    int getProblemCount();
    int getPercentGrade();
    const char *getLetterGrade();
    void setUserName(QString name);

private slots:

    void onUserName();
    void onGo();
    void onAnswer();
    void onStop();
    void onDefault();
    void onGradeLevel(int);

private:

    ///////////////////////////////////////////////////////////
    // Functions
    ///////////////////////////////////////////////////////////
    void initDisplay();
    void sendMessage(QString &line, msg_t level);
    void setAppStyles();
    void sendInfo(const char *line);
    void sendNotify(const char *line);
    void sendAlert(const char *line);
    void setEditable(bool state);
    void runTest();
    void getOps(int *pLop, int *pRop, QRect &limits);
    int getOneOp(int min, int max, int *pRepeats);
    void printProb(QPoint &ops, QString &qsOp);
    void updateUserFile();
    void genGrade();
    void openResultFile();
    void getDefaults();
    void setDefaults(QTextStream& in);
    void getMaxops();
    int openDefaults(QpFile& file, QTextStream& stream, bool builtin=false);
    void doProblem();

    ///////////////////////////////////////////////////////////
    // Data
    ///////////////////////////////////////////////////////////

    TButtonGroup <QCheckBox> *pTests;
    TEditGroup <QLineEdit> *pProblems;
    TEditGroup <QLineEdit> *pTimes;
    TEditGroup <QLineEdit> *pQuiz;
    TButtonGroup <QRadioButton> *pGradeLevel;
    QLineEdit *userNameEdit;
    QLabel *userNameLabel;
    QString userName;
    QPushButton *goButton;
    QPushButton *stopButton;
    QPushButton *defaultButton;
    QpFile *resultFile;
    QString resultFileName;
    QTextStream *resultStream;
    QMessageBox *pGradesPopup;
    Msg* pMsg;
    bool bRunning;              // Tests are running
    int testIndex;
    bool bFirstPass;

    int iGradeLevel;
    int iTotalCount;
    int iTotalRight;
    int iPercentGrade;
    const char *szLetterGrade;

    typedef struct _maxops {
        int iaLop[gl_boundary];
        int iaRop[gl_boundary];
    }
    MAXOPS;

    typedef struct {
        //
        // These members are initialized by pressing the "Start Tests" button.
        //
        bool bEmpty;            // No tests selected
        bool bOn;               // Test is ON or OFF ?
        op_t eOp;               // operator index
        int iCount;             // how many?
        int iSeconds;           // how much time to do it?
        bool bFirst;            // First pass? true or false?
        //
        //  The following are initialized or updated at test execution time
        //
        QString qsProblem;
        QElapsedTimer qdTimer;
        int iAnswer;
        int iPass;
        int iUserAnswer;
        int iUserTime;
        bool bCorrect;
        bool bOnTime;
        int iWrong;
        int iRight;
        int iScore;
        QString qsGrade;
        QRect  opLimits;
        MAXOPS maxops;
    } TESTPARM;

    TESTPARM testMatrix[op_boundary];
    RandOp rnd;
};

#endif // MATHTEST_H
