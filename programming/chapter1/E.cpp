#include "Function.hpp"
#include "EquationSolver.hpp"
#include <iostream>
#include <cmath>
using namespace std;
const double pi = acos(-1.);

//bisection method:
class F1:public Function{
        double operator() (double x) const{
            return 10*(0.5*pi-asin(x)-x*pow(1-x*x,0.5))-12.4;
    }
};

int main(){
    Bisection_Method h1(F1(), 0.0, 0.5, 1e-8,0.01);
    cout<<"the height of water is "<<h1.solve()<<endl;

    Newton_Method h2(F1(),0.0);
    cout<<"the height of water is "<<h2.solve()<<endl;

    Secant_Method h3(F1(),0.0 ,1,0 ,0.01);
    cout<<"the height of water is "<<h3.solve()<<endl;
    return 0;
}
