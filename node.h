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
    double getz();
    double getv();
    double getd();
    double getp();
    void setx(double x);
    void sety(double y);
    void setv(double v);
    void setz(double z);
    void setd(double d);
    void setp(double p);    //p is for pitch
    void update(double time);
    void reflect(int err);
    bool isIn(double x, double a, double b);
    node(int n);
    //variables:
    double x,y,z;
    double v;
    double d;
    double p;
    int lastErr = 0;
    int id;
    bool first = true;

};

#endif // NODE_H
