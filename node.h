#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QThread>
#include "publicvar.h"

class node:public QThread{
    Q_OBJECT
public:
    void run();
    double getx();
    double gety();
    double getv();
    double getd();
    void setx(double x);
    void sety(double y);
    void setv(double v);
    void setd(double d);
    void update(double time);
    void reflect(int err);
    bool isIn(double x, double a, double b);
    node(int n);
    //variables:
    double x,y;
    double v;
    double d;
    int lastErr = 0;
    int id;
    bool first = true;

};

#endif // NODE_H
