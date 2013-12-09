#include "tcombobox.h"

TComboBox::TComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void TComboBox::setText(const QString &text)
{
    QComboBox::setEditText(text);
}
