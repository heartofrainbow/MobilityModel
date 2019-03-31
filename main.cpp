#include "nodeplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NodePlot w;
    w.show();

    return a.exec();
}
