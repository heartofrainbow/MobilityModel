#include "nodeplot.h"
#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    NodePlot w;
    w.show();

    return a.exec();
}
