#include<iostream>
#include<fstream>
#include"Function.hpp"
#include"BSpline.h"
using namespace std;

class Runge:public Function{
    public:
    double operator() (double x) const {
        return 1/(1+x*x);
    }
};

vector<double> getknots(const double &a, const int &n, const int &degree){
    vector<double> v;
    for(int i=1; i<n+1+2*degree; ++i){
        v.push_back(a-degree+i);
    }
    return v;
}

void test(const vector<double> &v){
    Runge f;
    BSpline<3> b1(v,f);
    b1.print("output_C.json");
    /*BSpline<3> b2(v,f,boundaryType::complete);
    b2.print("output_C.json");

    BSpline<3> b3(v,f,boundaryType::periodic);
    b3.print("output_C.json");

    BSpline<3> b4(v,f,boundaryType::specified);
    b4.print("output_C.json");*/
}

int main(){
    vector<double> v2=getknots(-6.0, 11, 3);
    test(v2);
    return 0;
}