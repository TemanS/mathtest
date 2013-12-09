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

#ifndef CONTROLGROUP_H
#define CONTROLGROUP_H

//#include <QtCore>
//#include <QtGui>
#include <QtCore/QSignalMapper>
#include <QtCore/QStringBuilder>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QList>

// Debugging headers
//
//#include <QtCore/QtGlobal>  // qPrintable() - be careful with this, see docs
//#include <QtCore/QtDebug>   // qDebug()

enum signal_t {
    st_clicked,
    st_pressed,
    st_released,
    st_activated,
    st_returnPressed
};

typedef void (*pTfunc)();

/*
** class Twidget
**
** This class is only used by instances of the ControlGroup template class
** for creation and management of the QWidget based objects that it creates.
**
** Because we don't know the type of the objects that will be created by
** ControlGroup, we use a class template and pass the object type to be
** expanded by the compliler when the template is invoked.
**
** The QList of Objects will be maintained by each instance of ControlGroup
** for the object types that it creates.
**
** These objects must all have QWidget as their base class, since ControlGroup
** will operate on member functions from that class.
**
** The following fields must be initialized by the caller.
**
** objName      - Object name stem. The index of each object in the list will
**                be added to the name for each object
** objText      - QList of strings to be displayed on each object.
** sizes        - QList of QSize gives height & width of each object in pixels.
**                A single-item list means they are all the same size
** layout       - QList of QPoint for the location of each object in pixels.
** labelText    - QList of QStrings to be displayed on each label.
**                An empty list indicates no labels.
** labelSizes   - QList of QSize for height and width of each label
**                A single-item list means they are all the same size
** labelLayout  - QList of QPoint for the label layout
**
** --------------------------------------------------------------------------
** For TButtonGroup Objects Only
** TEditGroup Objexts have their own signal mapping method
** --------------------------------------------------------------------------
** connect      - If true, connect to a signal mapper
** grouped      - If both connect and grouped are true, controls will be
**                mutually exclusive and the signal mapper will be a
**                QButtonGroup.
S*/
class Twidget
{
public:
    // These fields must be initialized by the caller to ControlGroup
    //
    QString objName;                // object name stem of widget
    QList <QString> objText;        // text displayed on objects,
    QList <QSize>   sizes;          // A list of sizes for the objects
    QList <QPoint>  layout;         // A list of locations for the objects
    QList <int>     hOffset;        // A list of horizontal offsets
    QList <QString> labelText;      // Text displayed labels
    QList <QSize>   labelSizes;     // list of sizes for the labels
    QList <QPoint>  labelLayout;    // A list of locations for the objects
    bool grouped;                   // group these in a QButtonGroup
    bool connect;                   // true if a mapper is desired, else false
};

///////////////////////////////////////////////////////////////////////////////
//
// TButtonGroup
//
// A template class for control buttons of various types - QCheckBox,
// QPushButton, QRadioButton, QToolButton, etc.
//
// This class creates a group of button controls, sets their geometries
// as passed by the TWidget struct, places them on the parent QWidget,
// and, if requested, groups them or connects them to a signal mapper.
//
template <typename T>
class TButtonGroup : public QWidget
{
public:
    explicit TButtonGroup( Twidget *tw, QWidget *parent=0 )
    {
        init(tw, parent);
    }
    TButtonGroup(){};
    ~TButtonGroup(){};
    QList<T*> widgetList;
    QList<QLabel*> labelList;
    void init(Twidget *tw, QWidget *parent);
    QSignalMapper *mapper;
    QButtonGroup *buttonGroup;
    Twidget* getTwidget() {return m_tw;}

private:
    Twidget *m_tw;
    QWidget *m_parent;
};

template <typename T>
void TButtonGroup<T>::init( Twidget *tw, QWidget *parent )
{
    m_tw = tw;
    m_parent = parent;

    // See if the caller wants to connect a signal mapper.
    // If he does, see if he wants the buttons grouped (mutually exclusive)
    // The QButtonGroup emits a signal with the ID of the checked button for
    // each button that is checked in an exclusive group.
    // If the buttons are not mutually exclusive, then use a QSignalMapper
    //
    if(tw->connect)
    {
        if(tw->grouped)
            buttonGroup = new QButtonGroup(parent);
        else
            mapper = new QSignalMapper(parent);
    }

    int objCount = tw->layout.size();

    for (int index = 0; index < objCount; index++)
    {
        T *object = new T(parent);
        widgetList.append((T *)object);

        // Create the objectName, combining the text passed in a QString by the
        // caller and the index of the object in the QList tw->layout.size.
        // If the caller didn't pass a name, then the object will simply get
        // a number for its object name.
        //
        object->setObjectName(QString(tw->objName % QString::number(index)));

        // If the control group member has no text label, then skip this step.
        // If the caller only passes one label text string, then be sure that
        // we can only index to the zeroth element of the tw->objText QList.
        //
        if(tw->objText.size() > 0) {
            int textIndex = (tw->objText.size()) == 1 ? 0 : index;
            QString objText = tw->objText[textIndex];
            object->setText(objText);
            //qDebug() << "textIndex: " << textIndex << " objText: "
            //         << qPrintable(objText);
        }

        // If the caller only passes one size, then one size fits all.
        // Be sure that we can only index to the zeroth element of the
        // tw->sizes QList. When there is only one item in there, trying
        // to index beyond that throws an access violation, as well it
        // should.
        //
        int sizeIndex = (tw->sizes.size() == 1) ? 0 : index;
        int width = tw->sizes[sizeIndex].width();
        int height = tw->sizes[sizeIndex].height();

        int x = tw->layout[index].x();
        int y = tw->layout[index].y();

        // Layout the objects according to the Table Configuration given by
        // the "topology" field in the Twidget struct.
        //
        int hOff;
        int hOffsetIndex;
        if(tw->hOffset.size() > 0) {
            hOffsetIndex = tw->hOffset.size() == 1 ? 0 : index;
            hOff = tw->hOffset[hOffsetIndex];
        } else {
            hOff = 0;
        }

        object->setGeometry(x + hOff, y, width, height);

        // If the caller wants to map signals to this control group...
        // If the buttons are grouped, the buttonGroup will send signals.
        // In this case, the mapper only needs to be programmed at the end of
        // this loop.
        // If they are not grouped, the buttons themselves must notify the
        // mapper, and the mapper must be programmed to recognize each button
        // in the group.
        //
        if(tw->connect)
        {
            if(tw->grouped)
                buttonGroup->addButton(object, index);
            else
            {
                connect(object, SIGNAL(clicked()), mapper, SLOT(map()));
                mapper->setMapping(object, index);
            }
        }
    }

    for (int index = 0; index < tw->labelLayout.size(); index++)
    {
        QLabel *label = new QLabel(parent);

        // If the caller only passes one label text string, then be sure that
        // we can only index to the zeroth element of the tw->objText QList.
        //
        int textIndex = (tw->labelText.size()) == 1 ? 0 : index;
        QString labelText = tw->labelText[textIndex];
        label->setText(labelText);
        //qDebug() << "textIndex: " << textIndex << " objText: "
        //         << qPrintable(objText);

        // If the caller only passes one size, then one size fits all.
        // Be sure that we can only index to the zeroth element of the
        // tw->sizes QList. When there is only one item in there, trying
        // to index beyond that throws an access violation, as well it
        // should.
        //
        int sizeIndex = (tw->labelSizes.size() == 1) ? 0 : index;
        int width = tw->labelSizes[sizeIndex].width();
        int height = tw->labelSizes[sizeIndex].height();

        // Layout the labels according to the Table Configuration given by
        // the "topology" field in the Twidget struct.
        //
        int x = tw->labelLayout[index].x();
        int y = tw->labelLayout[index].y();

        int hOff;
        int hOffsetIndex;
        if(tw->hOffset.size() > 0) {
            hOffsetIndex = tw->hOffset.size() == 1 ? 0 : index;
            hOff = tw->hOffset[hOffsetIndex];
        } else {
            hOff = 0;
        }

        label->setGeometry(x + hOff, y, width, height);
        labelList.append(label);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// TEditGroup
//
// A template class for input widgets of various types - QLineEdit, QTextEdit,
// QComboBox, etc.
//
// IMPORTANT::
//  QComboBox must be sub-classed and the new class must implement a setText()
//  function by calling QComboBox::setEditText, because QComboBox does not
//  have a setText() function, neither does it appear to inherit it from any
//  other class.
//
// This class creates a group of edit controls, sets their geometries
// as passed by the TWidget struct, places them on the parent QWidget,
// and, if requested, connects them to a signal mapper.
//
template <typename T>
class TEditGroup : public QWidget
{
public:
    explicit TEditGroup( Twidget *tw, QWidget *parent=0 )
    {
        init(tw, parent);
    }
    TEditGroup(){};
    ~TEditGroup(){};
    QList<T*> widgetList;
    QList<QLabel*> labelList;
    QSignalMapper *mapper;
    Twidget* getTwidget() {return m_tw;}
    void init(Twidget *tw, QWidget *parent);

private:
    Twidget *m_tw;
    QWidget *m_parent;

};

template <typename T>
void TEditGroup<T>::init( Twidget *tw, QWidget *parent )
{
    m_tw = tw;
    m_parent = parent;

    int objCount = tw->layout.size();

    for (int index = 0; index < objCount; index++)
    {
        T *object = new T(parent);
        widgetList.append((T *)object);

        // Create the objectName, combining the text passed in a QString by the
        // caller and the index of the object in the QList tw->layout.size.
        //
        object->setObjectName(QString(tw->objName % QString::number(index)));

        // If the caller only passes one label text string, then be sure that
        // we can only index to the zeroth element of the tw->objText QList.
        //
        int textIndex = (tw->objText.size()) == 1 ? 0 : index;
        QString objText = tw->objText[textIndex];
        object->setText(objText);
        //qDebug() << "textIndex: " << textIndex << " objText: "
        //         << qPrintable(objText);

        // If the caller only passes one size, then one size fits all.
        // Be sure that we can only index to the zeroth element of the
        // tw->sizes QList. When there is only one item in there, trying
        // to index beyond that throws an access violation, as well it
        // should.
        //
        int sizeIndex = (tw->sizes.size() == 1) ? 0 : index;
        int width = tw->sizes[sizeIndex].width();
        int height = tw->sizes[sizeIndex].height();

        // Layout the objects according to the Table Configuration given by
        // the "topology" field in the Twidget struct.
        //
        int x = tw->layout[index].x();
        int y = tw->layout[index].y();

        int hOff;
        int hOffsetIndex;
        if(tw->hOffset.size() > 0) {
            hOffsetIndex = tw->hOffset.size() == 1 ? 0 : index;
            hOff = tw->hOffset[hOffsetIndex];
        } else {
            hOff = 0;
        }
        object->setGeometry(x + hOff, y, width, height);
    }

    for (int index = 0; index < tw->labelLayout.size(); index++)
    {
        QLabel *label = new QLabel(parent);

        // If the caller only passes one label text string, then be sure that
        // we can only index to the zeroth element of the tw->objText QList.
        //
        int textIndex = (tw->labelText.size()) == 1 ? 0 : index;
        QString labelText = tw->labelText[textIndex];
        label->setText(labelText);
        //qDebug() << "textIndex: " << textIndex << " objText: "
        //         << qPrintable(objText);

        // If the caller only passes one size, then one size fits all.
        // Be sure that we can only index to the zeroth element of the
        // tw->sizes QList. When there is only one item in there, trying
        // to index beyond that throws an access violation, as well it
        // should.
        //
        int sizeIndex = (tw->labelSizes.size() == 1) ? 0 : index;
        int width = tw->labelSizes[sizeIndex].width();
        int height = tw->labelSizes[sizeIndex].height();

        // Layout the labels according to the Table Configuration given by
        // the "topology" field in the Twidget struct.
        //
        int x = tw->labelLayout[index].x();
        int y = tw->labelLayout[index].y();

        int hOff;
        int hOffsetIndex;
        if(tw->hOffset.size() > 0) {
            hOffsetIndex = tw->hOffset.size() == 1 ? 0 : index;
            hOff = tw->hOffset[hOffsetIndex];
        } else {
            hOff = 0;
        }

        label->setGeometry(x + hOff, y, width, height);
        labelList.append(label);
    }
}

#endif // CONTROLGROUP_H
