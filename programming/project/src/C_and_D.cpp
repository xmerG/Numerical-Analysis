#include<iostream>
#include<fstream>
#include"Function.hpp"
#include"BSpline.h"
#include"ppForm.hpp"
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

void print(const vector<double> &e){
    cout<<"the errors at specific points are"<<" ";
    for(int i=0; i<e.size(); ++i){
        cout<<e[i]<<" ";
    }
    cout<<endl;
}

vector<double> p{-3.5, -3.0, -0.5, 0.0, 0.5, 3.0, 3.5};
template <int d>
void Error(BSpline<d> &b, const Function &f){
    vector<double> error;
    for(int i=0; i<p.size(); ++i){
        error.push_back(f(p[i])-b.calculateValue(p[i]));
    }
    print(error);
}

void linear_test(const vector<double> &v){
    Runge f;
    BSpline<1> b(v,f);
    b.print("output_C.json");
    Error(b, f);
}


vector<double> u{-5.0, -4.0, -3.0, -2.0, -1.0, 0, 1.0, 2.0, 3.0, 4.0, 5.0};

void cubic_test(const vector<double> &v){
    Runge f;
    BSpline<3> b1(v,f,boundaryType::natural);
    vector<double> error;
    b1.print("output_C.json");


    BSpline<3> b2(v,f,boundaryType::complete);
    b2.print("output_C.json");

    BSpline<3> b3(v,f,boundaryType::periodic);
    b3.print("output_C.json");

    BSpline<3> b4(v,f,boundaryType::specified);
    b4.print("output_C.json");

    BSpline<3> b5(v,f,boundaryType::not_a_knot);
    b5.print("output_C.json");
    
    Error(b1, f);
    Error(b2, f);
    Error(b3, f);
    Error(b4, f);
    Error(b5, f);
}

void test(const double &a, const int &b){
    vector<double> v1=getknots(a,b,1);
    linear_test(v1);

    vector<double> v2=getknots(a, b, 3);
    cubic_test(v2);
}

int main(){
    cout<<"-----------------------result for C and D---------------------------------"<<endl;
    test(-6.0,11);
    test(-5.5, 10);
    return 0;
}