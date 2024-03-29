#ifndef PUBLICVAR_H
#define PUBLICVAR_H

#include <QVector>
#include <random>
#include <QPointF>
#include <QScatterDataArray>
#include <Q3DScatter>

using namespace std;
extern QtDataVisualization::QScatterDataArray *points;
extern QtDataVisualization::QScatter3DSeries *series;

extern bool running;
extern int nNodes;
extern double XMAX;
extern double XMIN;
extern double YMAX;
extern double YMIN;
extern double VMIN;
extern double VMAX;
extern double ZMIN;
extern double ZMAX;
extern double interval;
extern double vmean;
extern double dmean;
extern double pmean;
extern double alpha;

//extern double xbuffer;
//extern double ybuffer;

extern random_device rd;
extern default_random_engine e;


#endif // PUBLICVAR_H
