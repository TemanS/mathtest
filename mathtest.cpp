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

/******************************************************************************
**
**    APPLICATION WINDOW LAYOUT
**    =========================
**
**    Problem         Number of       Time in       Level
**    Type            Problems        Seconds
**                    +----------+    +----------+
**    [X] Add         |   Edit   |    |   Edit   |  ( ) 1
**                    +----------+    +----------+
**                    +----------+    +----------+
**    [X] Subtract    |   Edit   |    |   Edit   |  (*) 2
**                    +----------+    +----------+
**                    +----------+    +----------+
**    [X] Multiply    |   Edit   |    |   Edit   |  ( ) 3
**                    +----------+    +----------+
**                    +----------+    +----------+
**    [X] Divide      |   Edit   |    |   Edit   |  ( ) 4
**                    +----------+    +----------+
**
**    +--------------+  +--------------+  +--------------+
**    |   Start PB   |  |   Stop PB    |  |    Dlg PB    |
**    +--------------+  +--------------+  +--------------+
**
**    +--------------------------------------------------+
**    |    Username                                      |
**    +--------------------------------------------------+
**
**    Problem                               Your
**    Number          Problem               Answer
**    +----------+    +----------------+    +------------+
**    | R/O Edit |    |    R/O Edit    |    |    Edit    |
**    +----------+    +----------------+    +------------+
**
**    +--------------------------------------------------+
**    |                                                  |
**    |                                                  |
**    |           Messages  R/O                          |
**    |                                                  |
**    |                                                  |
**    |                                                  |
**    |                                                  |
**    +--------------------------------------------------+
**
******************************************************************************/

#include <time.h>
#include <QtCore>
#include <QtGui>

#include <mathpack.h>

#include "defaultdlg.h"
#include "mathtest.h"

MathTest::MathTest(QPoint origin, QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::Window); // min, max and close buttons
    setGeometry(origin.x(), origin.y(), mt::winWid, mt::winHgt);
    QpAppStyle style;
    setStyleSheet(*style.getAppStyle());

    initDisplay();

    bRunning = false;
    bFirstPass = true;
    iTotalCount = 0;
    pGradesPopup = new QMessageBox;
    resultFileName = QString("");
    resultFile = new QpFile(pMsg);
    resultStream = new QTextStream(resultFile);

    getDefaults();
    getMaxops();
}

MathTest::~MathTest()
{
    delete resultStream;
    delete resultFile;
    delete pGradesPopup;
    delete pMsg;
}

void MathTest::onUserName()
{
    if((userNameEdit->text() != userName)
    && (userNameEdit->text() != "")) {
        openResultFile();
        getDefaults();
    }
}

void MathTest::openResultFile()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString dtStr = dt.toString("-yyyy.MM.dd-HH.mm.ss");
    QString suffix = dtStr % ".txt";

    userName = userNameEdit->text();

    if(userName != "")
        resultFileName = userName % "-mathtest" % suffix;
    else
        resultFileName = "test-mathtest" % suffix;

    resultFile->setFileName(resultFileName);

    if (! resultFile->open(QIODevice::Append | QIODevice::Text))
        pMsg->sendInfo("Continuing without saving results to a file.");
    else {
        *resultStream << userName << "\n" << dt.toString() << "\n\n";
    }
}

void MathTest::onStop()
{
    if(bRunning) {
        if(resultFile->exists(true) && resultFile->isOpen()) {
            resultStream->flush();
            resultFile->close();
        }
        testMatrix[testIndex].bFirst = false;
        bRunning = false;
        setEditable(true);
        pMsg->sendAlert("\"Stop\" Button Pressed - Test Stopped.");
    }
}

void MathTest::onDefault()
{
    // Instantiate child dialog
    //
    DefaultDlg *def = new DefaultDlg(this);

    // Send our data to the child dialog.
    //
    for(int i = 0; i < op_boundary; ++i) {
        def->pTests->widgetList[i]->setCheckState
                (pTests->widgetList[i]->checkState());

        def->pProblems->widgetList[i]->setText
                (pProblems->widgetList[i]->text());

        def->pTimes->widgetList[i]->setText(pTimes->widgetList[i]->text());
    }
    int grade = pGradeLevel->buttonGroup->checkedId();
    def->pGradeLevel->widgetList[grade]->setChecked(true);
    def->userNameEdit->setText(userNameEdit->text());

    /**********************************************************************
    ** Execute the child dialog modally.
    **********************************************************************/
    def->exec();

    // Get the data back from the child dialog.
    //
    int status;
    QpFile file(pMsg);
    QTextStream stream;

    grade = def->pGradeLevel->buttonGroup->checkedId();
    pGradeLevel->widgetList[grade]->setChecked(true);
    userNameEdit->setText(def->userNameEdit->text());

    if((status = openDefaults(file, stream)) == qpfile::fFailed)
        pMsg->sendInfo("Defaults will not be saved in a file.");

    for(int i = 0; i < op_boundary; ++i) {
        pTests->widgetList[i]->setCheckState
                (def->pTests->widgetList[i]->checkState());

        pProblems->widgetList[i]->setText
                (def->pProblems->widgetList[i]->text());

        pTimes->widgetList[i]->setText(def->pTimes->widgetList[i]->text());

        // If we were able to open or create a file to contain these
        // default values, then write then info into the file stream.
        //
        if(status != qpfile::fFailed) {
            int iCheckState = def->pTests->widgetList[i]->checkState();
            QString temp = QString("%1 ").arg(iCheckState);
            stream << temp;

            temp = QString("%1 ").arg(def->pProblems->widgetList[i]->text(), 3);
            stream << temp;

            temp = QString("%1 ").arg(def->pTimes->widgetList[i]->text(), 3);
            stream << temp << endl;
        }
    }

    if(status != qpfile::fFailed) {
        QString temp = QString("%1 ").arg(grade);
        stream << temp << endl;
        stream << def->userNameEdit->text() << endl;
    }

    if(file.exists(true))
        file.close();

    // Delete the child dialog.
    //
    delete def;
}

void MathTest::onGo()
{
    int iCount = 0;

    // Only do this if we're not already running.
    //
    if(bRunning) {
        pMsg->sendAlert("Tests are already running.");
    }
    else {
        iTotalCount = 0;
        iTotalRight = 0;
        iGradeLevel = pGradeLevel->buttonGroup->checkedId();

        // Initialize the test matrix and the qDebugother global control
        // parameters.
        //
        for(int i = 0; i < op_boundary; i++)
        {
            if(pTests->widgetList[i]->isChecked()) {
                testMatrix[i].bOn = true;
                testMatrix[i].eOp = (op_t)i;
                testMatrix[i].iCount = pProblems->widgetList[i]->text().toInt();
                testMatrix[i].iPass = 0;
                testMatrix[i].iSeconds = pTimes->widgetList[i]->text().toInt();
                testMatrix[i].bFirst = true;
                iCount++;
            }
            else {
                testMatrix[i].bOn = false;
                testMatrix[i].eOp = op_boundary;
                testMatrix[i].iCount = 0;
                testMatrix[i].iPass = 0;
                testMatrix[i].bFirst = false;
            }
            testMatrix[i].iRight = 0;
            testMatrix[i].iWrong = 0;
            testMatrix[i].iScore = 0;
            testMatrix[i].qsGrade = "";
        }

        if(iCount == 0) {
            pMsg->sendAlert("No tests selected.\nPlease select a test.");
        }
        else {

            // If we have a file for this session, reopen it now.
            //
            if(!bFirstPass && !resultFile->isOpen()
            && (userNameEdit->text() == userName)
            && (userName != "")) {
                if (! resultFile->open(QIODevice::Append | QIODevice::Text))
                    pMsg->sendInfo("Continuing without appending to the file.");
            }

            if(bFirstPass || (userNameEdit->text() != userName)) {
                openResultFile();
                getDefaults();
            }

            if(bFirstPass)
                bFirstPass = false;

            setEditable(false);
            bRunning = true;
            testIndex = 0;
            runTest();
        }
    }
}

void MathTest::onAnswer()
{
    if(bRunning == true) {
        TESTPARM *pTestParm = &testMatrix[testIndex];
        QString text;
        msg_t level;
        QString qsEt = QString("%1").arg(pTestParm->qdTimer.elapsed() / 1000);

        int iUserAnswer = pQuiz->widgetList[qz_answer]->text().toInt();
        int iTime = pTimes->widgetList[testIndex]->text().toInt();

        text = pTestParm->qsProblem % " = "
                % QString("%1").arg(pTestParm->iAnswer)
                % "  You said: "
                % pQuiz->widgetList[qz_answer]->text() % " ";

        if(iUserAnswer == pTestParm->iAnswer) {
            level = msg_notify;
            pTestParm->bCorrect = true;
            pTestParm->iRight++;
        }
        else {
            level = msg_alert;
            pTestParm->bCorrect = false;
            pTestParm->iWrong++;
        }

        pMsg->sendMessage(text, level, nocrlf);

        text = "in " % qsEt % " sec";

        if(qsEt.toInt() <= iTime) {
            if(!pTestParm->bCorrect)
                pTestParm->iWrong--;
            level = msg_notify;
            pTestParm->bOnTime = true;
        }
        else {
            if(pTestParm->bCorrect)
                pTestParm->iRight--;
            level = msg_alert;
            pTestParm->bOnTime = false;
        }

        level = qsEt.toInt() <= iTime ? msg_notify : msg_alert;
        pMsg->sendMessage(text, level);

        pTestParm->iUserAnswer = iUserAnswer;
        pTestParm->iUserTime = qsEt.toInt();

        if(resultFile->exists(true) && resultFile->isOpen())
            updateUserFile();

        pTestParm->iPass++;
        runTest();
    }
}

void MathTest::onGradeLevel(int gl)
{
    // This is redundant, because the onGo() routine reads the checkedId
    // of the GradeLevel radio button group anyway. But just to show how
    // a TButtonGroup event is connected to a slot, we will leave this here.
    //
    iGradeLevel = gl;
}

void MathTest::updateUserFile()
{
    QString qsLog;

    if(testMatrix[testIndex].iPass == 0)
    {
        qsLog = QString("\n%1 Problems, Timeout: %2 seconds, Grade Level %3\n")
                .arg(testMatrix[testIndex].iCount, 3)
                .arg(testMatrix[testIndex].iSeconds)
                .arg(iGradeLevel + 1)
        % "----------------------------------------------------\n"
        % "                          Your    Time \n"
        % "Problem          Answer  Answer  Seconds  Status\n"
        % "----------------------------------------------------\n";
        *resultStream << qsLog;
    }

    qsLog = QString("%1 %2  %3  %4  %5  ")
            .arg(testMatrix[testIndex].iPass+1, 4)
            .arg(testMatrix[testIndex].qsProblem, 10)
            .arg(testMatrix[testIndex].iAnswer, 6)
            .arg(testMatrix[testIndex].iUserAnswer,6)
            .arg(testMatrix[testIndex].iUserTime, 7);

    qsLog += testMatrix[testIndex].bCorrect ? "Correct  " : "Wrong    ";
    qsLog += !testMatrix[testIndex].bOnTime ? "Timed out!\n" : "\n";
    *resultStream << qsLog;
}

void MathTest::runTest()
{
    TESTPARM *pt = &testMatrix[testIndex];

    // If the Start button was not pressed, then don't do anything.
    //
    if (!bRunning) {
        setEditable(true);
        return;
    }

    // If we've completed one set of tests, update the total count and
    // the total correct answers, then generate the grade.
    //
    if((pt->bOn == true) && (pt->iCount > 0) && (pt->iPass == pt->iCount)) {
        iTotalCount += pt->iCount;
        iTotalRight += pt->iRight;
        genGrade();
    }

    // If the currently indexed test was not checked,
    // or if we've done all the problems for the currently indexed test,
    // then increment the test indext to the next test type.
    // If we've done all the tests, do some housekeeping and return.
    //
    if ((pt->bOn == false)
    ||  (pt->iPass >= pt->iCount)) {

        // See if there are any more tests to run.
        //
        while(++testIndex < op_boundary && testMatrix[testIndex].bOn == false)
            ;

        if(testIndex >= op_boundary) {
            bRunning = false;
            setEditable(true);
            genGrade();
            pMsg->sendInfo("Tests complete.");

            if(resultFile->exists(true) && resultFile->isOpen()) {
                resultStream->flush();
                resultFile->close();
            }
            return;
        }
        else {
            testMatrix[testIndex].iPass = 0;
        }
    }

    QString opStr;
    QRect limits;
    QPoint ops;
    RandOp rnd;

    switch((int)testMatrix[testIndex].eOp)
    {
    case op_add:
        limits.setRect(1, maxops[op_add].iaLop[iGradeLevel],
                       1, maxops[op_add].iaRop[iGradeLevel]);
        rnd.getPair(ops, limits);
        testMatrix[testIndex].iAnswer  = ops.x() + ops.y();
        opStr = " + ";
        break;

    case op_sub:
        limits.setRect(1, maxops[op_sub].iaLop[iGradeLevel],
                       1, maxops[op_sub].iaRop[iGradeLevel]);
        rnd.getPair(ops, limits, true);
        testMatrix[testIndex].iAnswer = ops.x() - ops.y() ;
        opStr = " - ";
        break;

    case op_mul:
        limits.setRect(1, maxops[op_mul].iaLop[iGradeLevel],
                       1, maxops[op_mul].iaRop[iGradeLevel]);
        rnd.getPair(ops, limits);
        testMatrix[testIndex].iAnswer = ops.x() * ops.y() ;
        opStr = " * ";
        break;

    case op_div:
        ops.ry() = rnd.getOneUnique(1, maxops[op_div].iaRop[iGradeLevel]);
        ops.rx() = rnd.getOne(1, maxops[op_div].iaLop[iGradeLevel]) * ops.y();
        testMatrix[testIndex].iAnswer = ops.x() / ops.y();
        opStr = " / ";
        break;
    }
    printProb(ops, opStr);
}

///////////////////////////////////////////////////////////////////////////////
//
// CMathTestWDlg::GenGrade
//
void MathTest::genGrade()
{
    QString qsMsg;
    msg_t level;

    if(bRunning) {
        int iScore;
        const char *szGrade;
        TESTPARM* pTestParm = &testMatrix[testIndex];
        MpScore score(pTestParm->iCount, pTestParm->iRight);
        iScore = score.getPercentGrade();
        szGrade = score.getLetterGrade();
        qsMsg = QString("Score: %1%2 Grade %3\n")
                .arg(iScore).arg('%').arg(szGrade);

        if(resultFile->exists(true) && resultFile->isOpen())
            *resultStream << qsMsg;

        level = iScore > 75 ? msg_notify : msg_alert;
        pMsg->sendMessage(qsMsg, level);

        pTestParm->iScore = iScore;
        pTestParm->qsGrade = QString(szGrade);
    }
    else {
        MpScore score(iTotalCount, iTotalRight);
        iPercentGrade = score.getPercentGrade();
        szLetterGrade = score.getLetterGrade();
        qsMsg = QString("\nFinal score: %1%2  Final Grade: %3")
                .arg(iPercentGrade).arg('%').arg(szLetterGrade);

        if(resultFile->exists(true) && resultFile->isOpen())
            *resultStream << qsMsg << "\n\n";

        level = iPercentGrade > 75 ? msg_notify : msg_alert;
        pMsg->sendMessage(qsMsg, level);

        qsMsg = QString("Final score: %1%2").arg(iPercentGrade).arg('%');
        pGradesPopup->setText(qsMsg);
        qsMsg = QString("Final Grade: %1").arg(szLetterGrade);
        pGradesPopup->setInformativeText(qsMsg);
        pGradesPopup->exec();
    }

    // Now clean up the Problem Display windows.
    //
    for(int i = 0; i < qz_boundary; i++)
        pQuiz->widgetList[i]->setText("");
}

void MathTest::printProb(QPoint &ops, QString &qsOp)
{
    QString lopStr = QString("%1").arg(ops.x());
    QString ropStr = QString("%1").arg(ops.y());
    QString qsNmbr = QString("%1").arg(testMatrix[testIndex].iPass + 1) % ".";

    testMatrix[testIndex].qsProblem = lopStr % qsOp % ropStr;
    pQuiz->widgetList[qz_number]->setText(qsNmbr);
    pQuiz->widgetList[qz_quiz]->setText(testMatrix[testIndex].qsProblem);
    pQuiz->widgetList[qz_answer]->setFocus();
    pQuiz->widgetList[qz_answer]->clear();
    pQuiz->widgetList[qz_answer]->setCursorPosition(0);
    testMatrix[testIndex].qdTimer.start();
}

void MathTest::setEditable(bool state)
{
    int index;

    for(index = 0; index < op_boundary; index++)
    {
        pTests->widgetList[index]->setEnabled(state);
        pProblems->widgetList[index]->setEnabled(state);
        pTimes->widgetList[index]->setEnabled(state);
    }
}

void MathTest::initDisplay()
{
    int index;

    // Set the starting coordinates in pixels for the objects that will be
    // laid out.
    //
    const int tests_x = 20;     // x coord of left side of groups
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
    tw->sizes   << QSize(text_w+10, box_h);
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
    int gl_x = time_x + box_w + hs - 10;
    int gl_y = time_y;
    LAYOUT(gl_x, gl_y, 0, vs);
    tw->layout << CELL(0,0) << CELL(0,1) << CELL(0,2) << CELL(0,3);
    tw->sizes << QSize(box_w - 10, box_h);
    tw->objText << "1" << "2" << "3" <<"4";
    tw->connect = true;
    tw->grouped = true;
    pGradeLevel = new TButtonGroup <QRadioButton>(tw, this);
    pGradeLevel->widgetList[gl_3]->setChecked(true);
    connect(pGradeLevel->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(onGradeLevel(int)));

    // Layout the UserName box and its label
    //
    int userName_x = tests_x;
    int userName_y = tests_y + text_h + (4 * vs);
    int userNameEd_x = userName_x + text_w;
    int userNameEd_w = 250;
    userNameLabel = new QLabel(this);
    userNameLabel->setGeometry(userName_x, userName_y, text_w, text_h);
    userNameLabel->setText("Your Name:");
    userNameEdit = new QLineEdit(this);
    userNameEdit->setGeometry(userNameEd_x, userName_y, userNameEd_w, box_h);
    userNameEdit->setText("");
    userNameEdit->setCursorPosition(0);
    connect(userNameEdit, SIGNAL(returnPressed()), this, SLOT(onUserName()));

    // Create and connect the Start button.
    // Because this pushbutton's parent is QDialog, it will have its
    // autoDefault property set when instantiated. This can cause some very
    // confusing behavior if you don't know this. See "autoDefault" in the
    // QPushButton class reference. We will explicitly set this property
    // to FALSE for this button, and also set its "default" property to
    // FALSE.
    //
    const int pbw = 100;
    const int pbhs = 110;
    int goButton_x = tests_x+10;
    int goButton_y = userName_y + vs + 6;
    goButton = new QPushButton("Start", this);
    goButton->setGeometry(goButton_x, goButton_y, pbw, box_h);
    goButton->setAutoDefault(false);
    goButton->setDefault(false);
    connect(goButton, SIGNAL(clicked()), this, SLOT(onGo()));

    // Create and connect the stop button.
    //
    int stopButton_x = goButton_x + pbhs;
    int stopButton_y = goButton_y;
    stopButton = new QPushButton("Stop", this);
    stopButton->setGeometry(stopButton_x, stopButton_y, pbw, box_h);
    stopButton->setAutoDefault(false);
    stopButton->setDefault(false);
    connect(stopButton, SIGNAL(clicked()), this, SLOT(onStop()));

    // Create and connect the Default button.
    //
    int defaultButton_x = stopButton_x + pbhs;
    int defaultButton_y = goButton_y;
    defaultButton = new QPushButton("Defaults", this);
    defaultButton->setGeometry(defaultButton_x, defaultButton_y, pbw, box_h);
    defaultButton->setAutoDefault(false);
    defaultButton->setDefault(false);
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(onDefault()));

    // Layout the Quiz and Answer Boxes and their labels
    //
    tw = new Twidget;
    int quiz_x = tests_x + 20;
    int quiz_y = goButton_y + vs;
    LAYOUT(quiz_x, quiz_y, hs + box_w+10, text_h)
    tw->labelLayout << CELL(0,0) << CELL(1,0) << CELL(2,0)
                    << CELL(0,1) << CELL(1,1) << CELL(2,1);
    tw->labelText   << "Problem" << "The" << "Your"
                    << "Number" << "Problem" << "Answer";
    tw->labelSizes  << QSize(text_w, text_h);
    LAYOUT(quiz_x, quiz_y+(2*text_h)+2, hs + box_w+10, 0)
    tw->objName = "Quiz";
    tw->objText << "";
    tw->sizes   << QSize(box_w, box_h)
                << QSize(box_w+30, box_h)
                << QSize(box_w+10, box_h);
    tw->hOffset << 20 << -5 << 0 << 20 << -5 << 0;
    tw->layout  << CELL(0,0) << CELL(1,0) << CELL(2,0);
    pQuiz = new TEditGroup <QLineEdit>(tw, this);
    //
    // Set the Quiz Box to ReadOnly and connect the returnPressed
    // signal from the Answer Box.
    //
    pQuiz->widgetList[qz_number]->setReadOnly(true);
    pQuiz->widgetList[qz_number]->setAlignment(Qt::AlignRight);
    pQuiz->widgetList[qz_quiz]->setReadOnly(true);
    pQuiz->widgetList[qz_answer]->setEnabled(true);
    pQuiz->widgetList[qz_answer]->setInputMask("0000");
    pQuiz->widgetList[qz_answer]->setCursor(Qt::IBeamCursor);
    pQuiz->widgetList[qz_answer]->setCursorPosition(0);
    QLineEdit *tempLineEdit = pQuiz->widgetList[qz_answer];
    connect (tempLineEdit, SIGNAL(returnPressed()),
        this, SLOT(onAnswer()));

    // Create the Message Box
    //
    int msg_x = tests_x;
    int msg_y = quiz_y + vs + (3 * text_h);
    int msg_w = 340;
    int msg_h = 180;
    QTextEdit* messages = new QTextEdit(this);
    messages->setGeometry(msg_x, msg_y, msg_w, msg_h);
    messages->setReadOnly(true);
    messages->setFocusPolicy(Qt::ClickFocus);
    messages->setUpdatesEnabled(true);
    pMsg = new Msg(messages);
}

int MathTest::openDefaults(QpFile& inFile, QTextStream& stream, bool builtin)
{
    // Default Test Parameters
    // =====================================================
    //
    //  Test Selection, number of problems, and time allowed
    //
    //                      Number of   Time in
    //              Test    Problems    Seconds
    //  Add           2         10          10
    //  Subtract      2         10          15
    //  Multiply      2         10          25
    //  Divide        2         10          30
    //
    //  Grade Level   1 (2nd grade level)
    //
    //  userName "" (NULL)
    //
    QString defStr = QString(
                "2 10 10 \n"
                "2 10 15 \n"
                "2 10 25 \n"
                "2 10 30 \n"
                "1 \n "
                );

    QString defaultFileName;
    int status;

    if(userNameEdit->text() != "")
        defaultFileName = "mt-" % userNameEdit->text() % ".txt";
    else
        defaultFileName = "mt-default.txt";

    QFlags<QIODevice::OpenModeFlag>
        flags = QIODevice::ReadWrite | QIODevice::Text;

    inFile.setQuietOnSuccess(true);
    if((status = inFile.get(defaultFileName, flags)) != qpfile::fFailed)
        stream.setDevice(&inFile);
    else {
        stream.setString(&defStr);
        pMsg->sendInfo("Using built-in default test parameters.");
    }

    if((status == qpfile::fCreated) && builtin)
        stream << defStr;

    stream.seek(0);
    return status;
}

void MathTest::getDefaults()
{
    QTextStream in;
    QpFile file(pMsg);

    openDefaults(file, in, true);
    setDefaults(in);
    if(file.exists())
        file.close();
}

void MathTest::setDefaults(QTextStream& in)
{
    QString buff;

    // Get the Test Parameters from the text stream
    //
    for(int i = 0; i < op_boundary; ++i) {
        in >> buff;
        pTests->widgetList[i]->setChecked(QString(buff).toInt());
        in >> buff;
        pProblems->widgetList[i]->setText(buff);
        in >> buff;
        pTimes->widgetList[i]->setText(buff);
    }

    // Get the Grade Level
    //
    in >> buff;
    pGradeLevel->widgetList[QString(buff).toInt()]->setChecked(true);

    // Get the userName, but only if there isn't one in the username edit box.
    //
    in >> buff;
    if(userNameEdit->text() == "")
        userNameEdit->setText(buff);
}

void MathTest::getMaxops()
{
    // The following are the default maxima for left and right operands
    // for the respective arithmetic operations and grade levels.
    //
    //  Max Operand Values for the given grade levels
    //
    //  Grade Level 1       2       3       4
    //
    //  Add Lop    10      50     100    1000
    //      Rop    10      10     100     500
    //  Sub Lop    10      20     100    1000
    //      Rop    10      10     100     500
    //  Mul Lop     3      10      13      20
    //      Rop     3      10      13      20
    //  Div Lop     3      10      13      20
    //      Rop     3      10      13      20
    //
    QString qsMaxops = QString(
                "10 10 50 10 100 100 1000 500 \n"
                "10 10 50 10 100 100 1000 500 \n"
                "10  3 10 10  15  15   20  20 \n"
                "10  3 10 10  15  15   20  20 \n"
                );

    int status;
    QString buff;
    QTextStream stream;
    QpFile maxopsFile(pMsg);
    QString maxopsFileName = "mt-maxops.txt";

    QFlags<QIODevice::OpenModeFlag>
        flags = QIODevice::ReadWrite | QIODevice::Text;

    if((status = maxopsFile.get(maxopsFileName, flags, true)) != qpfile::fFailed)
        stream.setDevice(&maxopsFile);
    else {
        stream.setString(&qsMaxops);
        //pMsg->sendInfo("Using built-in default test parameters.");
    }

    if(status == qpfile::fCreated)
        stream << qsMaxops;

    stream.seek(0);

    // Get the Operand Maxima
    //
    int j, k;
    for(j = 0; j < op_boundary; ++j) {
        for(k = 0; k < gl_boundary; ++k) {
            stream >> buff;
            maxops[j].iaLop[k] = QString(buff).toInt();
            stream >> buff;
            maxops[j].iaRop[k] = QString(buff).toInt();
        }
    }
    if(maxopsFile.exists())
        maxopsFile.close();
}

int MathTest::getProblemCount()
{
    return iTotalCount;
}

int MathTest::getPercentGrade()
{
    return iPercentGrade;
}

const char *MathTest::getLetterGrade()
{
    return szLetterGrade;
}

void MathTest::setUserName(QString name)
{
    userNameEdit->setText(name);
    onUserName();
}
