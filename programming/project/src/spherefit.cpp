#include<iostream>
#include<cmath>
#include<vector>
#include"Function.hpp"
#include"Curve_fitting.hpp"
using namespace std;
const double pi = acos(-1.);
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

void Fit(const int &n, const double &a, const double &b, const Function &f1,
                const Function &f2,const string &filename){
    sphereFit fit1(n,a,b,f1,f2,knotsType::cumulate_chordal);
    fit1.cubic_ppfit(boundaryType::periodic);
    fit1.print(filename);

    sphereFit fit2(n,a,b,f1,f2,knotsType::uniform);
    fit2.cubic_ppfit(boundaryType::periodic);
    fit2.print(filename);

    sphereFit fit3(n,a,b,f1,f2,knotsType::cumulate_chordal);
    fit3.cubic_ppfit(boundaryType::natural);
    fit3.print(filename);

    sphereFit fit4(n,a,b,f1,f2,knotsType::uniform);
    fit4.cubic_ppfit(boundaryType::natural);
    fit4.print(filename);
}



int main(){
    F3_x f5;
    F3_y f6;
    Fit(40,0.0,2*pi,f5,f6)
    return 0;
}