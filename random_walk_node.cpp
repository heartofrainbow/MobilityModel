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
#include <QPointF>


//using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

random_walk_node::random_walk_node(int n):node(n){
    this->id = n;
}

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
        if (this->z <=ZMIN){
            this->z = 2*ZMIN - this->z;
            reflect(5);
        }else if(this->z >= ZMAX){
            this->z = 2*ZMAX - this->z;
            reflect(6);
        }
        this->x += this->getv()*cos(this->getd())*sin(this->getp())*time;
        this->y += this->getv()*sin(this->getd())*sin(this->getp())*time;
        this->z += this->getv()*cos(this->getp())*time;
        points->replace(id,QVector3D(this->x,this->y,this->getz()));
//        series->dataProxy()->resetArray(points);
}

void random_walk_node::reflect(int err){      //err: 1 XMIN 2 XMAX 3 YMIN 4 YMAX
       double dd = this->getd();
       double pp = this->getp();
       if (err == lastErr) {           //In case random_walk_node reflect forever near an edge
           return;
       }
       if (err ==1 || err == 2) {
           this->setd(M_PI-dd);
           lastErr = err;
       } else if (err ==3 || err ==4) {
           this->setd(2*M_PI-dd);
           lastErr = err;
       } else if (err ==5 || err == 6) {
           this->p = M_PI - pp;
           lastErr = err;

}
}

void random_walk_node::run(){
    uniform_real_distribution<double> randomX(XMIN,XMAX);
    uniform_real_distribution<double> randomY(YMIN,YMAX);
    uniform_real_distribution<double> randomZ(ZMIN,ZMAX);
    uniform_real_distribution<double> randomVel(VMIN,VMAX);
    uniform_real_distribution<double> randomDir(0,2*M_PI);
    uniform_real_distribution<double> randomPitch(0,M_PI);
    QString str;
    x = randomX(e);
    y = randomY(e);
    z = randomZ(e);
    v = randomVel(e);
    d = randomDir(e);
    p = randomPitch(e);
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
    points->replace(id,QVector3D(this->getx(),this->gety(),this->getz()));
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
        lastUpdate = high_resolution_clock::now();
        currentTime = high_resolution_clock::now();
        changeTime = currentTime-lastChange;
        if(changeTime.count() > interval*1e6){       //Regenerate random_walk_node info every $timeout second
            lastChange = currentTime;
            this->setv(randomVel(e));
            this->setd(randomDir(e));
            this->setp(randomPitch(e));
            lastErr = 0;                //To avoid random_walk_node bouncing near the edge
        }
        msleep(10);
    }
}

