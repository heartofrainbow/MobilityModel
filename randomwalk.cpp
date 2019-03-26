#define M_PI 3.141592654
#include "randomwalk.h"
#include "ui_randomwalk.h"

#include <iostream>
#include <random>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <thread>


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>


static QVector<QPointF> points;
//static QGraphicsScene* scene;

<<<<<<< HEAD
static QVector<double> xx(0);
static QVector<double> yy(0);

static bool running = false;
static int nNodes = 0;
=======
bool running = false;
int nNodes = 0;
>>>>>>> parent of 1c12a77... A little change in UI and slightly improved robustness
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

static double XMAX = 100;
static double XMIN = 0;
static double YMAX = 100;
static double YMIN = 0;
static double VMIN = 10;
static double VMAX = 20;

static random_device rd;
static default_random_engine e(rd());



double node::getx(){
    return x;
}
double node::gety(){
    return y;
}
double node::getv(){
    return v;
}
double node::getd(){
    return d;
}
void node::setx(double x){
    this->x = x;
}
void node::sety(double y){
    this->y = y;
}
void node::setv(double v){
    this->v = v;
}
void node::setd(double d){
    if(d<0){
        d += 2*M_PI;
    }else if(d>=2*M_PI){
        d -= 2*M_PI;
    }
    this->d = d;
}
void node::update(double time){   //parameter time is in second
        if (this->x <= XMIN) {      //If node hit the XMIN bound then reflect
            this->x = 2*XMIN - this->x;     //Fix out of bound bug: if OutOfBound then mirror
            reflect(1);
        }else if(this->x >= XMAX){  //If node hit the XMAX bound then reflect
            this->x = 2*XMAX - this->x;
            reflect(2);
        }
        if (this->y <= YMIN) {
            this->y = 2*YMIN - this->y;
            reflect(3);
        } else if(this->y >= YMAX){
            this->y = 2*YMAX - this->y;
            reflect(4);
        }
        this->x += this->getv()*cos(this->getd())*time;
        this->y += this->getv()*sin(this->getd())*time;
        points.replace(id,QPointF(this->x,this->y));
}

void node::reflect(int err){      //err: 1 XMIN 2 XMAX 3 YMIN 4 YMAX
       double dd = this->getd();
       if (err == lastErr) {           //In case node reflect forever near an edge
           return;
       }
       if (err ==1 || err == 2) {
           this->setd(dd<=M_PI/2?(M_PI-dd):(3*M_PI-dd));
           lastErr = err;
       } else if (err ==3 || err ==4) {
           this->setd(2*M_PI-dd);
           lastErr = err;
       }
}

void node::run(){
    uniform_real_distribution<double> randomX(XMIN,XMAX);
    uniform_real_distribution<double> randomY(YMIN,YMAX);
    uniform_real_distribution<double> randomVel(VMIN,VMAX);
    uniform_real_distribution<double> randomDir(0,2*M_PI);
    QString str;
    x = randomX(e);
    y = randomY(e);
    v = randomVel(e);
    d = randomDir(e);
//    double interval = 1.0;        //Update node information every $interval second
    double timeout = 10.0;        //Change node speed&direction every $interval second
    std::chrono::duration<double, std::micro> tmpTime;     //Time between current and lastshow
    std::chrono::duration<double, std::micro> loopTime;    //Time used for a single loop
    std::chrono::duration<double, std::micro> changeTime;
    std::chrono::duration<double, std::micro> wholeTime;
    high_resolution_clock::time_point loopEndTime;
    cout<<setiosflags(ios::fixed);
    high_resolution_clock::time_point baseTime = high_resolution_clock::now();
    high_resolution_clock::time_point lastChange = high_resolution_clock::now();    //Time when node info changed
    high_resolution_clock::time_point  lastUpdate = lastChange;   //Time when node loc updated
    high_resolution_clock::time_point lastShow = lastUpdate;    //Time when node info shown
    high_resolution_clock::time_point currentTime = high_resolution_clock::now();
    wholeTime = currentTime-baseTime;
    //printf("ID: %d\tTime: %.8f s\tLOC: x=%.8f\ty=%.8f\t\tVEL=%.8f\t\tDIR=%.8f\n",(int)QThread::currentThreadId(), wholeTime.count()/1e6,this->getx(),this->gety(),this->getv(),this->getd());
    //str = QString("ID: %1\tTime: %2 s\tLOC: x=%3\ty=%4\t\tVEL=%5\t\tDIR=%6\n").arg(id).arg(wholeTime.count()/1e6,0,'f',8).arg(this->getx(),0,'f',8).arg(this->gety(),0,'f',8).arg(this->getv(),0,'f',8).arg(this->getd(),0,'f',8);
    points.replace(id,QPointF(this->getx(),this->gety()));
//    emit(output(str));
    //initial output
    while(running == true){
        currentTime = high_resolution_clock::now();
        tmpTime = (currentTime-lastShow);
        loopEndTime = high_resolution_clock::now();
        if (first) {
            lastUpdate = loopEndTime;
            lastChange = loopEndTime;
            first = false;
        }
        loopTime=(loopEndTime-lastUpdate);
        this->update(loopTime.count()/1e6);
//        if(tmpTime.count() >= interval*1e6){    //Show node info every $interval second
//            wholeTime = currentTime - baseTime;
//            //printf("ID: %d\tTime: %.8f s\tLOC: x=%.8f\ty=%.8f\t\tVEL=%.8f\t\tDIR=%.8f\n",(int)QThread::currentThreadId(), wholeTime.count()/1e6,this->getx(),this->gety(),this->getv(),this->getd());
////            str = QString("ID: %1\tTime: %2 s\tLOC: x=%3\ty=%4\t\tVEL=%5\t\tDIR=%6\n").arg(id)\
////                    .arg(wholeTime.count()/1e6,0,'f',8).arg(this->getx(),0,'f',8).arg(this->gety(),0,'f',8).arg(this->getv(),0,'f',8).arg(this->getd(),0,'f',8);
////            emit(output(str));
//            lastShow = high_resolution_clock::now();
//        }
        lastUpdate = high_resolution_clock::now();
        currentTime = high_resolution_clock::now();
        changeTime = currentTime-lastChange;
        if(changeTime.count() > timeout*1e6){       //Regenerate node info every $timeout second
            lastChange = currentTime;
            this->setv(randomVel(e));
            this->setd(randomDir(e));
            lastErr = 0;                //To avoid node bouncing near the edge
        }
        usleep(100);
    }
}
RandomWalk::RandomWalk(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RandomWalk)
{
    ui->setupUi(this);
    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::white);
    myScatter.setSize(5);
    ui->customPlot->graph(0)->setScatterStyle(myScatter);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
}

//node::node(QObject *parent):QThread(parent){

//}

node::node(int n){
    this->id = n;
}
RandomWalk::~RandomWalk()
{
    delete ui;
}

//void RandomWalk::on_OutputReceived(QString qs){
////    ui->textBrowser->insertPlainText(qs);
////    ui->textBrowser->moveCursor(QTextCursor::End);
//    QApplication::processEvents();
//}
void RandomWalk::on_pushButton_clicked()
{
    XMIN = ui->input_XMIN->text().toDouble();
    XMAX = ui->input_XMAX->text().toDouble();
    YMIN = ui->input_YMIN->text().toDouble();
    YMAX = ui->input_YMAX->text().toDouble();
    VMIN = ui->input_VMIN->text().toDouble();
    VMAX = ui->input_VMAX->text().toDouble();

    ui->customPlot->xAxis->setRange(XMIN, XMAX);
    ui->customPlot->yAxis->setRange(YMIN, YMAX);
    ui->customPlot->replot();
    showNodes *shower = new showNodes();
    connect(shower,SIGNAL(flushNodes()),this,SLOT(on_FlushNodes()));
    nNodes = ui->input_nNodes->text().toInt();
    running = true;
    QVector<double> x(nNodes);
    QVector<double> y(nNodes);
    points.resize(nNodes);
    for(int i=0;i<nNodes;i++){
        node *nd = new node(i);
        QThread::usleep(10);
//        connect(nd,SIGNAL(output(QString)),this,SLOT(on_OutputReceived(QString)));
        nd->start();
    }
    shower->start();
}

void RandomWalk::on_pushButton_2_clicked()
{
    running = false;
}

void RandomWalk::on_FlushNodes(){

   //Do Something
    QVector<double> x(nNodes),y(nNodes);
    for(int i=0;i<nNodes; i++){
<<<<<<< HEAD
//        xx[i]=points.at(i).x();
//        yy[i]=points.at(i).y();
        xx.replace(i,points.at(i).x());
        yy.replace(i,points.at(i).y());
=======
        x[i]=points.at(i).x();
        y[i]=points.at(i).y();
>>>>>>> parent of 1c12a77... A little change in UI and slightly improved robustness
    }
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->replot();
    x.clear();
    y.clear();
}
void showNodes::run(){
    while(running == true){
        emit(flushNodes());
        msleep(100);
    }
}
