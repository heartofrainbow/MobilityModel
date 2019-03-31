#ifndef RANDOM_DIRECTION_NODE_H
#define RANDOM_DIRECTION_NODE_H

#include <QObject>
#include <QThread>

class random_direction_node : public QThread
{
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
    random_direction_node(int n);
private:
    double x,y;
    double v;
    double d;
    int lastErr = 0;
    int id;
    bool first = true;
    bool reflected = false;
};

#endif // RANDOM_DIRECTION_NODE_H
