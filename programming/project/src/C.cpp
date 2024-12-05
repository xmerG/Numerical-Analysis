#include<iostream>
#include<fstream>
#include"Function.hpp"
#include"BSpline.hpp"
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

void linear_test(const vector<double> &v){
    Runge f;
    BSpline<1> b(v,f);
    b.print("output_C.txt");
}

void cubic_test(const vector<double> &v){
    Runge f;
    BSpline<3> b1(v,f);
    b1.print("output_C.txt");
    BSpline<3> b2(v,f,boundaryType::complete);
    b2.print("output_C.txt");

    BSpline<3> b3(v,f,boundaryType::periodic);
    b3.print("output_C.txt");

    BSpline<3> b4(v,f,boundaryType::specified);
    b4.print("output_C.txt");
}

void test(const double &a, const int &b){
    vector<double> v1=getknots(a,b,1);
    linear_test(v1);

    vector<double> v2=getknots(a, b, 3);
    cubic_test(v2);
}

int main(){
    test(-6.0,11);
    test(-5.5, 10);
    return 0;
}