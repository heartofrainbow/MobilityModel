#ifndef RANDOM_WAYPOINT_NODE_H
#define RANDOM_WAYPOINT_NODE_H

#include <QObject>
#include "node.h"

class random_waypoint_node : public node
{
    Q_OBJECT
public:
    void run();
    void update(double time);
    void reflect();
    //node(QObject *parent=NULL);
    random_waypoint_node(int n);
private:
    double destx;
    double desty;
};

#endif // RANDOM_WAYPOINT_NODE_H
