#define M_PI 3.141592654
#include "node.h"
#include "publicvar.h"

void node::run(){

}
node::node(int n){
//    this->id = n;
}

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
bool node::isIn(double x, double a, double b){
    if(a > b){
        double tmp = a;
        a = b;
        b = tmp;
    }
    if(a <= x && x <= b){
        return true;
    }return false;
}
