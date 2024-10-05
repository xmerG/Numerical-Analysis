#include "Function.hpp"
#include "EquationSolver.hpp"
#include <iostream>
#include <cmath>
using namespace std;

const double pi = acos(-1.);

class F1 : public Function {
public:
    double operator() (double x) const {
        return sin(x/2)-1;
    }
};


class F2 : public Function {
public:
    double operator() (double x) const {
        return exp(x)-tan(x);
    }
};

class F3 : public Function {
public:
    double operator() (double x) const {
        return pow(x,3)-12*pow(x,2)+3*x+1;
    }
};

int main(){
    Secant_Method root1(F1(), 0.0, pi/2);
    cout<<"the root of sin(x/2)-1  with x0=0 x1=pi/2 is  "<<root1.solve()<<endl;
    
    Secant_Method root1a(F1(), pi/2, pi);
    cout<<"the root of sin(x/2)-1  with x0=pi/2 x1=pi is  "<<root1a.solve()<<endl;
    

    Secant_Method root2(F2(), 1.0, 1.4);
    cout<<"the root of e^x-tanx with x0=1,x1=1.4 is  "<<root2.solve()<<endl;

    Secant_Method root2a(F2(), 0.5, 1.4);
    cout<<"the root of e^x-tanx with x0=0.5,x1=1.4 is  "<<root2a.solve()<<endl;
    
    Secant_Method root3(F3(), 0.0, -0.5);
    cout<<"the root of x^3-12x^2+3x+1  with x0=0, x1=-0.5 is "<<root3.solve()<<endl;

       Secant_Method root3a(F3(), -1.5, -1.0);
    cout<<"the root of x^3-12x^2+3x+1  with x0=-1.5, x1=-1.0 is  "<<root3a.solve()<<endl;

    
    return 0;
}
