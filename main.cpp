#include "tjftp.h"
#include "common.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TJFTP w;
    w.show();

    return a.exec();
}
