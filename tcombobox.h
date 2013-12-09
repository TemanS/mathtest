#ifndef TCOMBOBOX_H
#define TCOMBOBOX_H
#include <QComboBox>

class TComboBox : public QComboBox
{
public:
    explicit TComboBox(QWidget *parent = 0);
    void setText(const QString &text);

signals:

public slots:

};

#endif // TCOMBOBOX_H
