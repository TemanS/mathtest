#include <QtGui/QApplication>
#include "mathtest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathTest w;
    w.show();

    return a.exec();
}
