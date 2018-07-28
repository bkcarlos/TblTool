#include "tbltool.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TblTool w;
    w.show();

    return a.exec();
}
