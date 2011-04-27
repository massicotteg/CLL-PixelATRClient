#include <QtGui/QApplication>
#include "pixelatrclientwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PixelATRClientWindow w;
    w.show();

    return a.exec();
}
