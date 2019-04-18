#define M_PI 3.141592654
#include "random_waypoint_node.h"
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
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

random_waypoint_node::random_waypoint_node(int n):node(n){
    this->id = n;
}

void random_waypoint_node::update(double time){   //parameter time is in second
    if(x < destx && (d >= M_PI/2 && d <= 3*M_PI/2)){
        reflect();
        return;
    }else if(x > destx && (d<= M_PI/2 || d >= 3*M_PI/2)){
        reflect();
        return;
    }else if(d == M_PI/2 && y > desty){
        reflect();
        return;
    }else if(d == 3*M_PI/2 && y < desty){
        reflect();
        return;
    }
        this->x += this->getv()*cos(this->getd())*time;
        this->y += this->getv()*sin(this->getd())*time;
        points->replace(id,QVector3D(this->x,this->y,0));
}

void random_waypoint_node::reflect(){      //err: 1 XMIN 2 XMAX 3 YMIN 4 YMAX
    this->x = destx;
    this->y = desty;
    points->replace(id,QVector3D(this->x,this->y,0));
    sleep(interval);
    lastErr=1;
}

void random_waypoint_node::run(){
    uniform_real_distribution<double> randomX(XMIN,XMAX);
    uniform_real_distribution<double> randomY(YMIN,YMAX);
    uniform_real_distribution<double> randomVel(VMIN,VMAX);
    uniform_real_distribution<double> randomDir(0,2*M_PI);
    QString str;
    x = randomX(e);
    y = randomY(e);
    destx = randomX(e);
    desty = randomY(e);
    v = randomVel(e);
    d = atan2(desty-y,destx-x);
    std::chrono::duration<double, std::micro> tmpTime;     //Time between current and lastshow
    std::chrono::duration<double, std::micro> loopTime;    //Time used for a single loop
    std::chrono::duration<double, std::micro> changeTime;
    std::chrono::duration<double, std::micro> wholeTime;
    high_resolution_clock::time_point loopEndTime;
    cout<<setiosflags(ios::fixed);
    high_resolution_clock::time_point baseTime = high_resolution_clock::now();
    high_resolution_clock::time_point lastChange = high_resolution_clock::now();    //Time when random_waypoint_node info changed
    high_resolution_clock::time_point  lastUpdate = lastChange;   //Time when random_waypoint_node loc updated
    high_resolution_clock::time_point lastShow = lastUpdate;    //Time when random_waypoint_node info shown
    high_resolution_clock::time_point currentTime = high_resolution_clock::now();
    wholeTime = currentTime-baseTime;
    points->replace(id,QVector3D(this->getx(),this->gety(),0));
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
        if(lastErr == 1){       //Regenerate random_waypoint_node info every $timeout second
            lastChange = currentTime;
            destx = randomX(e);
            desty = randomY(e);
            this->setv(randomVel(e));
            this->setd(atan2(desty-y,destx-x));
            lastErr = 0;                //To avoid random_waypoint_node bouncing near the edge
        }
        msleep(10);
    }
}

