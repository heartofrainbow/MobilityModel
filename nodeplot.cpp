#define M_PI 3.141592654
#include "nodeplot.h"
#include "ui_nodeplot.h"
#include "publicvar.h"
#include "random_walk_node.h"
#include "random_direction_node.h"
#include "random_waypoint_node.h"
#include "gauss_markov_node.h"

#include <iostream>
#include <random>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <thread>
#include <QDebug>

#include <QGraphicsView>
#include <QPointF>
#include <QVector>
//#include <QtCharts/QScatterSeries>
//#include <QtCharts/QLegendMarker>
#include <QtDataVisualization>
#include <Q3DScatter>
#include <qmessagebox.h>
#include <utility>
#define __NO_STD_VECTOR


using namespace QtDataVisualization;



QScatterDataArray *points = new QScatterDataArray();
Q3DScatter *graph;
QScatter3DSeries *series;

bool running = false;
int nNodes = 0;
int type = 0;   //0=RandomWalk, 1=RandomDirection
double interval = 2;


using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

double XMAX = 100;
double XMIN = 0;
double YMAX = 100;
double YMIN = 0;
double ZMIN = 0;
double ZMAX = 100;
double VMIN = 10;
double VMAX = 20;
double vmean = 15;
double dmean = 3.141592654;
double pmean = 1.570796327;
double alpha = 0.5;
double xbuffer = (XMAX-XMIN)/10;
double ybuffer = (YMAX-YMIN)/10;
double zbuffer = (ZMAX-ZMIN)/10;

random_device rd;
default_random_engine e(rd());



NodePlot::NodePlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NodePlot)
{
    ui->setupUi(this);
    graph = new Q3DScatter();
    series = new QScatter3DSeries;
    graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetBehindLow);
    graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    graph->axisX()->setRange(0, 480);
    graph->axisY()->setRange(0, 480);
    graph->axisZ()->setRange(0, 480);
//    graph->axisX()->setSegmentCount(int(480/20));
//    graph->axisY()->setSegmentCount(int(480/20));
//    graph->axisZ()->setSegmentCount(int(480/20));
    graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    graph->axisX()->setTitle("X");
    graph->axisY()->setTitle("Y");
    graph->axisZ()->setTitle("Z");
    graph->axisX()->setTitleVisible(true);
    graph->axisY()->setTitleVisible(true);
    graph->axisZ()->setTitleVisible(true);
//    graph->axisX()->setAutoAdjustRange(true);
//    graph->axisY()->setAutoAdjustRange(true);
//    graph->axisZ()->setAutoAdjustRange(true);
    series->setItemSize(0.05);
    QWidget *container = QWidget::createWindowContainer(graph);
    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
    }
    QHBoxLayout *hLayout = new QHBoxLayout(ui->widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    points->resize(0);
    series->dataProxy()->addItems(*points);
    graph->addSeries(series);
    graph->removeSeries(series);
    series->dataProxy()->addItems(*points);
    graph->addSeries(series);

}

NodePlot::~NodePlot()
{
    delete ui;
}

void NodePlot::on_pushButton_clicked()
{
    if(running == true){
//        running = false;    //In case OK buttom was clicked more than once
//        points.clear();
//        xx.clear();
//        yy.clear();
        QMessageBox::information(this,"提示","在运行下一次仿真时请先点击停止按钮以停止本次仿真！",QMessageBox::Ok);
        return;
    }
    points->clear();
    //ui->customPlot->graph(0)->setData(xx, yy);
    //ui->customPlot->replot();
    XMIN = ui->input_XMIN->text().toDouble();
    XMAX = ui->input_XMAX->text().toDouble();
    YMIN = ui->input_YMIN->text().toDouble();
    YMAX = ui->input_YMAX->text().toDouble();
    ZMIN = ui->input_ZMIN->text().toDouble();
    ZMAX = ui->input_ZMAX->text().toDouble();
    VMIN = ui->input_VMIN->text().toDouble();
    VMAX = ui->input_VMAX->text().toDouble();
    interval = ui->input_interval->text().toDouble();
    vmean = ui->input_VMEAN->text().toDouble();
    dmean = ui->input_DMEAN->text().toDouble();
    pmean = ui->input_PMEAN->text().toDouble();
    alpha = ui->input_alpha->text().toDouble();
    xbuffer = (XMAX-YMIN)/10;
    ybuffer = (YMAX-YMIN)/10;
    zbuffer = (ZMAX-ZMIN)/10;
   // ui->customPlot->xAxis->setRange(XMIN, XMAX);
    //ui->customPlot->yAxis->setRange(YMIN, YMAX);
    //ui->customPlot->replot();
    graph->axisX()->setRange(XMIN-xbuffer, XMAX+xbuffer);
    graph->axisY()->setRange(YMIN-ybuffer, YMAX+ybuffer);
    graph->axisZ()->setRange(ZMIN-zbuffer, ZMAX+zbuffer);
    showNodes *shower = new showNodes();
    connect(shower,SIGNAL(flushNodes()),this,SLOT(on_FlushNodes()));
    nNodes = ui->input_nNodes->text().toInt();
    running = true;
    points->resize(nNodes);
    if(type == 0){
        for(int i=0;i<nNodes;i++){
            random_walk_node *nd = new random_walk_node(i);
//            QThread::usleep(10);
            nd->start();
        }
    }else if(type ==1){
        for(int i=0;i<nNodes;i++){
            random_direction_node *nd = new random_direction_node(i);
//            QThread::usleep(10);
            nd->start();
        }
    }else if(type == 2){
        for(int i=0;i<nNodes;i++){
                    random_waypoint_node *nd = new random_waypoint_node(i);
//                    QThread::usleep(10);
                    nd->start();
        }
    }else if(type ==3){
        for(int i=0;i<nNodes;i++){
            gauss_markov_node *nd = new gauss_markov_node(i);
//            QThread::usleep(10);
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
//    for(int i=0;i<nNodes; i++){
//        xx.replace(i,points.at(i).x());
//        yy.replace(i,points.at(i).y());
//    }
   // ui->customPlot->graph(0)->setData(xx, yy);
    //ui->customPlot->replot();
    series->dataProxy()->resetArray(points);
}
void showNodes::run(){
    while(running == true){
        emit(flushNodes());
        msleep(50);
    }
}

void NodePlot::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0){
        type = 0;
    }else if(index == 1){
        type = 1;
    }else if(index == 2){
        type = 2;
    }else if(index == 3){
        type = 3;
    }
}
