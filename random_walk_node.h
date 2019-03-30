#ifndef RANDOM_WALK_NODE_H
#define RANDOM_WALK_NODE_H

#include <QObject>
#include <QThread>

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
    //node(QObject *parent=NULL);
    node(int n);
//signals:
//    void output(QString qs);
private:
    double x,y;
    double v;
    double d;
    int lastErr = 0;
    int id;
    bool first = true;
};
#endif // RANDOM_WALK_NODE_H
