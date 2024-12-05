#include<iostream>
#include<vector>
#include<cmath>
#include"Function.hpp"
#include"BSpline.hpp"
#include"ppForm.hpp"
using namespace std;

double distance(const vector<double> &x, const vector<double> &y){
    if(x.size()!=y.size()){
        cerr<<"not same dimension points"<<endl;
        return -1;
    }
    else{
        double d=0.0;
        for(int i=0; i<x.size(); ++i){
            d+=pow(x[i]-y[i],2);
        }
        return sqrt(d);
    }
}

vector<double> getknots(const vector<vector<double>> &points){
    double t=0.0;
    int n=points.size();
    vector<double> knots(n,0.0);
    for(int i=0; i<n-1; ++i){
        t+=distance(points[i],points[i+1]);
        knots[i+1]=t;
    }
    return knots;
}

