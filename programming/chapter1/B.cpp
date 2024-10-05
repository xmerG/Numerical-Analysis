#include "Function.hpp"
#include "EquationSolver.hpp"
#include <iostream>
#include <cmath>
using namespace std;

const double pi = acos(-1.);

class F1 : public Function {
public:
    double operator() (double x) const {
        return 1/x-tan(x);
    }
};


class F2 : public Function {
public:
    double operator() (double x) const {
        return 1/x-pow(2,x);
    }
};

class F3 : public Function {
public:
    double operator() (double x) const {
        return pow(2,-x)+exp(x)+2*cos(x)-6;
    }
};

class F4 : public Function {
public:
    double operator() (double x) const {
        return (pow(x,3)+4*pow(x,2)+3*x+5)/(2*pow(x,3)-9*pow(x,2)+18*x-2);
    }
};
int main(){
    Bisection_Method root1(F1(), 0.0, pi/2);
    cout<<"the root of x^{-1}-tan(x) on [0,pi/2] is "<<root1.solve()<<endl;
    
    Bisection_Method root2(F2(), 0.0, 1.0);
    cout<<"the root of 1/x-2^x  on [0,1]is  "<<root2.solve()<<endl;
    
    Bisection_Method root3(F3(), 1.0, 3.0);
    cout<<"the root of 2^{-x}+e^x+2cosx-6 on [1,3]  is  "<<root3.solve()<<endl;

    Bisection_Method root4(F4(), 0.0, 4.0);
    cout<<"the root of (x^3+4x^2+3x+5)/(2x^3-9x^2+18x-2) is  "<<root4.solve()<<endl;
    return 0;
}