#include<iostream>
#include<cmath>
#include<vector>
#include"Function.hpp"
#include"Curve_Fitting.hpp"

using namespace std;

const double pi = acos(-1.);
const double c=sqrt(3);

class F1_x:public Function{
public:
    double operator()(double x) const{
        return sin(x)+x*cos(x);
    }
};

class F1_y:public Function{
public:
    double operator()(double x) const{
        return cos(x)-x*sin(x);
    }
};




class F2_x:public Function{
public:
    double operator()(double x) const{
        return c*cos(x);
    }
};

class F2_y:public Function{
public:
    double operator()(double x) const{
        return 2.0*(c*sin(x)+sqrt(c*abs(cos(x))))/3.0;
    }
};

class F3_x:public Function{
public:
    double operator()(double x) const{
        double a=cos(x);
        return sin(a)*cos(sin(x))/(1+cos(a));
    }
};


class F3_y:public Function{
public:
    double operator()(double x) const{
        double a=cos(x);
        return sin(a)*sin(sin(x))/(1+cos(a));
    }
};


int main(){
    F1_x f1;
    F1_y f2;
    Fit(10, 0, 6*pi, f1, f2,"E_curve_r2.txt");
    Fit(40, 0, 6*pi, f1, f2,"E_curve_r2.txt");
    Fit(160, 0, 6*pi, f1, f2,"E_curve_r2.txt");


    F2_x f3;
    F2_y f4;
    Fit(10, -pi, pi, f3, f4,"E_curve_r1.txt");
     
    Fit(40, -pi, pi, f3, f4,"E_curve_r1.txt");

    Fit(160, -pi, pi, f3, f4,"E_curve_r1.txt");

    F3_x f5;
    F3_y f6;
    sphereFit fit1(40,0.0,2*pi,f5,f6);
    fit1.cubic_ppfit();
    fit1.print("E_spherecurve.txt");
    return 0;
}