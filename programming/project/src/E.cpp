#include<iostream>
#include<cmath>
#include<vector>
#include"Function.hpp"
#include"BSpline.hpp"
#include"ppForm.hpp"
#include"Curve_Fitting.hpp"

using namespace std;

const double pi = acos(-1.);
const double a=sqrt(3);

class F1_x:public Function{
public:
    double operator()(const double &x){
        return sin(x)+x*cos(x);
    }
};

class F1_y:public Function{
public:
    double operator()(const double &x){
        return cos(x)-x*sin(x);
    }
};

vector<double> get_t(const int &N, const double &a, const double &b){
    vector<double> t(N+1);
    for(int i=0; i<N+1; ++i){
        t[i]=i+(b-a)/N;
    }
    return t;
}

vector<double> getvals(const vector<double> &t, const Function &f){
    int n=t.size();
    vector<double> vals(n);
    for(int i=0; i<n; ++i){
        vals[i]=f(t[i]);
    }
    return vals;
}

vector<vector<double>> getpoints(const vector<double> &x, const vector<double> &y){
    if(x.size()!=y.size()){
        cerr<<"not points"<<endl;
        return;
    }
    else{
        int n=x.size();
        vector<vector<double>> points(n);
        for(int i=0; i<n; ++i){
            points[i].push_back(x[i]);
            points[i].push_back(y[i]);
        }
        return points;
    }
}