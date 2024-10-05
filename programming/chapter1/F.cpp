#include "Function.hpp"
#include "EquationSolver.hpp"
#include <iostream>
#include <cmath>
using namespace std;

const double pi = acos(-1.);

class F:public Function{
    private:
        double l, h, D, beta;
    public:
        F(double _l, double _h, double  _D, double  _beta):l(_l), h(_h), D(_D), beta(_beta){}
        double operator() (double x) const {
            double A, B, C, E;
            double beta1=pi*beta/180;
            A=l*sin(beta1);
            B=l*cos(beta1);
            C=(h+0.5*D)*sin(beta1)-0.5*D*tan(beta1);
            E=(h+0.5*D)*cos(beta1)-0.5*D;
            return A*sin(x)*cos(x)+B*pow(sin(x),2)-C*cos(x)-E*sin(x);            
    }
};

void Solve1(){
    cout<<"(a) under the condition, alpha is "<<endl;
    double x0=30*pi/180;
    Newton_Method a1(F(89.0, 49.0, 55.0, 11.5),x0);
    double alpha1=a1.solve();
    double alpha=alpha1*180/pi;
    cout<<alpha<<" degree"<<endl;
};

void Solve2(){
    cout<<"(b) under the condition, alpha is "<<endl;
    double x0=33*pi/180;
    Newton_Method a2(F(89.0, 49.0, 30.0, 11.5),x0);
    double alpha2=a2.solve();
    double alpha=alpha2*180/pi;
    cout<<alpha<<" degree"<<endl;
};

void Solve3(double x0, double x1){
    cout<<"(c) under the condition, alpha is "<<endl;
    double _x0=x0*pi/180;
    double _x1=x1*pi/180;
    Secant_Method a3(F(89.0, 49.0, 55.0, 11.5),_x0,_x1);
    double alpha3=a3.solve();
    double alpha=alpha3*180/pi;
    cout<<alpha<<" degree"<<endl;
};

int main(){
    Solve1();
    Solve2();
    Solve3(3.0,5.0);
    Solve3(174.0,179.0);
    return 0;
}