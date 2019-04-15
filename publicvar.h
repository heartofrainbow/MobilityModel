#ifndef PUBLICVAR_H
#define PUBLICVAR_H

#include <QVector>
#include <random>
#include <QPointF>

using namespace std;
extern QVector<QPointF> points;

extern QVector<double> xx;
extern QVector<double> yy;

extern bool running;
extern int nNodes;
extern double XMAX;
extern double XMIN;
extern double YMAX;
extern double YMIN;
extern double VMIN;
extern double VMAX;
extern double interval;
extern double vmean;
extern double dmean;
extern double alpha;

extern random_device rd;
extern default_random_engine e;


#endif // PUBLICVAR_H
