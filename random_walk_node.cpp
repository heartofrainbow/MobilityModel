#define M_PI 3.141592654
#include "random_walk_node.h"
#include "publicvar.h"
#include <iostream>
#include <random>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <thread>
#include "node.h"
#include <QDebug>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

//random_device rd;
//default_random_engine e(rd());
random_walk_node::random_walk_node(int n):node(n){
    this->id = n;
}

//double random_walk_node::getx(){
//    return x;
//}
//double random_walk_node::gety(){
//    return y;
//}
//double random_walk_node::getv(){
//    return v;
//}
//double random_walk_node::getd(){
//    return d;
//}
//void random_walk_node::setx(double x){
//    this->x = x;
//}
//void random_walk_node::sety(double y){
//    this->y = y;
//}
//void random_walk_node::setv(double v){
//    this->v = v;
//}
//void random_walk_node::setd(double d){
//    if(d<0){
//        d += 2*M_PI;
//    }else if(d>=2*M_PI){
//        d -= 2*M_PI;
//    }
//    this->d = d;
//}
void random_walk_node::update(double time){   //parameter time is in second
        if (this->x <= XMIN) {      //If random_walk_node hit the XMIN bound then reflect
            this->x = 2*XMIN - this->x;     //Fix out of bound bug: if OutOfBound then mirror
            reflect(1);
        }else if(this->x >= XMAX){  //If random_walk_node hit the XMAX bound then reflect
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

void random_walk_node::reflect(int err){      //err: 1 XMIN 2 XMAX 3 YMIN 4 YMAX
       double dd = this->getd();
       if (err == lastErr) {           //In case random_walk_node reflect forever near an edge
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

void random_walk_node::run(){
    uniform_real_distribution<double> randomX(XMIN,XMAX);
    uniform_real_distribution<double> randomY(YMIN,YMAX);
    uniform_real_distribution<double> randomVel(VMIN,VMAX);
    uniform_real_distribution<double> randomDir(0,2*M_PI);
    QString str;
    x = randomX(e);
    y = randomY(e);
    v = randomVel(e);
    d = randomDir(e);
//    double interval = 1.0;        //Update random_walk_node information every $interval second
    double timeout = 10.0;        //Change random_walk_node speed&direction every $interval second
    std::chrono::duration<double, std::micro> tmpTime;     //Time between current and lastshow
    std::chrono::duration<double, std::micro> loopTime;    //Time used for a single loop
    std::chrono::duration<double, std::micro> changeTime;
    std::chrono::duration<double, std::micro> wholeTime;
    high_resolution_clock::time_point loopEndTime;
    cout<<setiosflags(ios::fixed);
    high_resolution_clock::time_point baseTime = high_resolution_clock::now();
    high_resolution_clock::time_point lastChange = high_resolution_clock::now();    //Time when random_walk_node info changed
    high_resolution_clock::time_point  lastUpdate = lastChange;   //Time when random_walk_node loc updated
    high_resolution_clock::time_point lastShow = lastUpdate;    //Time when random_walk_node info shown
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
//        if(tmpTime.count() >= interval*1e6){    //Show random_walk_node info every $interval second
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
        if(changeTime.count() > timeout*1e6){       //Regenerate random_walk_node info every $timeout second
            lastChange = currentTime;
            this->setv(randomVel(e));
            this->setd(randomDir(e));
            lastErr = 0;                //To avoid random_walk_node bouncing near the edge
        }
        msleep(10);
    }
}

