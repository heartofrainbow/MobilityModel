#define M_PI 3.141592654
#include "randomwalk.h"
#include "ui_randomwalk.h"

#include <iostream>
#include <random>
#include <math.h>
#include <iomanip>
#include <chrono>
//#include <thread>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const double XMAX = 100;
const double XMIN = 0;
const double YMAX = 100;
const double YMIN = 0;
const double VMIN = 10;
const double VMAX = 20;

random_device rd;
default_random_engine e(rd());
uniform_real_distribution<double> randomX(XMIN,XMAX);
uniform_real_distribution<double> randomY(YMIN,YMAX);
uniform_real_distribution<double> randomVel(VMIN,VMAX);
uniform_real_distribution<double> randomDir(0,2*M_PI);

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
}

void node::reflect(int err){      //err: 1 XMIN 2 XMAX 3 YMIN 4 YMAX
       double dd = this->getd();
       if (err == lastErr) {           //In case node reflect forever near an edge
           return;
       }
       if (err ==1 || err == 2) {
           this->setd(dd<=M_PI/2?(M_PI-dd):(3*M_PI-dd));
           lastErr = err;
       } else if (err ==3 | err ==4) {
           this->setd(2*M_PI-dd);
           lastErr = err;
       }
}

void node::run(){
    x = randomX(e);
    y = randomY(e);
    v = randomVel(e);
    d = randomDir(e);
    double interval = 1.0;        //Update node information every $interval second
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
    printf("ID: %d\tTime: %.8f s\tLOC: x=%.8f\ty=%.8f\t\tVEL=%.8f\t\tDIR=%.8f\n",(int)QThread::currentThreadId(), wholeTime.count()/1e6,this->getx(),this->gety(),this->getv(),this->getd());
    //initial output
    while(true){
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
        if(tmpTime.count() >= interval*1e6){    //Show node info every $interval second
            wholeTime = currentTime - baseTime;
            printf("ID: %d\tTime: %.8f s\tLOC: x=%.8f\ty=%.8f\t\tVEL=%.8f\t\tDIR=%.8f\n",(int)QThread::currentThreadId(), wholeTime.count()/1e6,this->getx(),this->gety(),this->getv(),this->getd());
            lastShow = high_resolution_clock::now();
        }
        lastUpdate = high_resolution_clock::now();
        currentTime = high_resolution_clock::now();
        changeTime = currentTime-lastChange;
        if(changeTime.count() > timeout*1e6){       //Regenerate node info every $timeout second
            lastChange = currentTime;
            this->setv(randomVel(e));
            this->setd(randomDir(e));
            lastErr = 0;                //To avoid node bouncing near the edge
        }

    }
}
RandomWalk::RandomWalk(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RandomWalk)
{
    ui->setupUi(this);
}

node::node(QObject *parent):QThread(parent){

}
RandomWalk::~RandomWalk()
{
    delete ui;
}


void RandomWalk::on_pushButton_clicked()
{
    node *nd = new node(this);
    nd->start();
}
