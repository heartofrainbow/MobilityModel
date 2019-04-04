#ifndef RANDOM_WALK_NODE_H
#define RANDOM_WALK_NODE_H

#include <QObject>
#include <QThread>
#include "node.h"
#include "publicvar.h"

class random_walk_node:public node{
    Q_OBJECT
public:
    void run();
    void update(double time);
    void reflect(int err);
    //node(QObject *parent=NULL);
    random_walk_node(int n);
//signals:
//    void output(QString qs);
//private:
//    double x,y;
//    double v;
//    double d;
//    int lastErr = 0;
//    int id;
//    bool first = true;
};
#endif // RANDOM_WALK_NODE_H
