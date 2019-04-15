#define M_PI 3.141592654
#include "gauss_markov_node.h"
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

gauss_markov_node::gauss_markov_node(int n):node(n){
    this->id = n;
}

void gauss_markov_node::update(double time){   //parameter time is in second
        if (this->x <= XMIN) {      //If gauss_markov_node hit the XMIN bound then reflect
            this->x = 2*XMIN - this->x;     //Fix out of bound bug: if OutOfBound then mirror
            reflect(1);
        }else if(this->x >= XMAX){  //If gauss_markov_node hit the XMAX bound then reflect
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

void gauss_markov_node::reflect(int err){      //err: 1 XMIN 2 XMAX 3 YMIN 4 YMAX
       double dd = this->getd();
       if (err == lastErr) {           //In case gauss_markov_node reflect forever near an edge
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

void gauss_markov_node::run(){
    uniform_real_distribution<double> randomX(XMIN,XMAX);
    uniform_real_distribution<double> randomY(YMIN,YMAX);
    uniform_real_distribution<double> randomVel(VMIN,VMAX);
    uniform_real_distribution<double> randomDir(0,2*M_PI);
    normal_distribution<double> randomVr(0,1);
    normal_distribution<double> randomDr(0,1);
    QString str;
    x = randomX(e);
    y = randomY(e);
    v = randomVel(e);
    d = randomDir(e);
    double timeout = interval;        //Change gauss_markov_node speed&direction every $interval second
    std::chrono::duration<double, std::micro> tmpTime;     //Time between current and lastshow
    std::chrono::duration<double, std::micro> loopTime;    //Time used for a single loop
    std::chrono::duration<double, std::micro> changeTime;
    std::chrono::duration<double, std::micro> wholeTime;
    high_resolution_clock::time_point loopEndTime;
    cout<<setiosflags(ios::fixed);
    high_resolution_clock::time_point baseTime = high_resolution_clock::now();
    high_resolution_clock::time_point lastChange = high_resolution_clock::now();    //Time when gauss_markov_node info changed
    high_resolution_clock::time_point  lastUpdate = lastChange;   //Time when gauss_markov_node loc updated
    high_resolution_clock::time_point lastShow = lastUpdate;    //Time when gauss_markov_node info shown
    high_resolution_clock::time_point currentTime = high_resolution_clock::now();
    wholeTime = currentTime-baseTime;
    points.replace(id,QPointF(this->getx(),this->gety()));
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
        if(changeTime.count() > timeout*1e6){       //Regenerate gauss_markov_node info every $timeout second
            lastChange = currentTime;
//            this->setv(randomVel(e));
//            this->setd(randomDir(e));
            v = alpha*v + (1-alpha)*vmean + sqrt(1-alpha*alpha)*randomVr(e);
            d = alpha*d + (1-alpha)*dmean + sqrt(1-alpha*alpha)*randomDr(e);
            lastErr = 0;                //To avoid gauss_markov_node bouncing near the edge
        }
        msleep(10);
    }
}

