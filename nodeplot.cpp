#define M_PI 3.141592654
#include "nodeplot.h"
#include "ui_nodeplot.h"
#include "publicvar.h"
#include "random_walk_node.h"
#include "random_direction_node.h"

#include <iostream>
#include <random>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <thread>


#include <QGraphicsView>
#include <QPointF>
#include <QVector>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLegendMarker>
#include <QDebug>



QVector<QPointF> points;
QVector<double> xx(0);
QVector<double> yy(0);
bool running = false;
int nNodes = 0;
int type = 0;   //0=RandomWalk, 1=RandomDirection

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

double XMAX = 100;
double XMIN = 0;
double YMAX = 100;
double YMIN = 0;
double VMIN = 10;
double VMAX = 20;

random_device rd;
default_random_engine e(rd());



NodePlot::NodePlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NodePlot)
{
    ui->setupUi(this);
    ui->customPlot->addGraph();
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssDisc);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::white);
    myScatter.setSize(10);
    ui->customPlot->graph(0)->setScatterStyle(myScatter);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);

}

//node::node(QObject *parent):QThread(parent){

//}


NodePlot::~NodePlot()
{
    delete ui;
}

//void NodePlot::on_OutputReceived(QString qs){
////    ui->textBrowser->insertPlainText(qs);
////    ui->textBrowser->moveCursor(QTextCursor::End);
//    QApplication::processEvents();
//}
void NodePlot::on_pushButton_clicked()
{
    if(running == true){
        running = false;    //In case OK buttom was clicked more than once
    }
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
    points.resize(nNodes);
    xx.resize(nNodes);
    yy.resize(nNodes);
    if(type == 0){
        for(int i=0;i<nNodes;i++){
            random_walk_node *nd = new random_walk_node(i);
            QThread::usleep(10);
    //        connect(nd,SIGNAL(output(QString)),this,SLOT(on_OutputReceived(QString)));
            nd->start();
        }
    }else if(type ==1){
        for(int i=0;i<nNodes;i++){
            random_direction_node *nd = new random_direction_node(i);
            QThread::usleep(10);
            nd->start();
        }
    }
    shower->start();
}

void NodePlot::on_pushButton_2_clicked()
{
    running = false;
//    points.clear();
//    xx.clear();
//    yy.clear();
}

void NodePlot::on_FlushNodes(){

   //Do Something

    for(int i=0;i<nNodes; i++){
//        xx[i]=points.at(i).x();
//        yy[i]=points.at(i).y();
        xx.replace(i,points.at(i).x());
        yy.replace(i,points.at(i).y());
    }
    ui->customPlot->graph(0)->setData(xx, yy);
    ui->customPlot->replot();
//    x.clear();
//    y.clear();
}
void showNodes::run(){
    while(running == true){
        emit(flushNodes());
        msleep(100);
    }
}

void NodePlot::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        type = 0;
    }else if(index == 1){
        type = 1;
    }
}
