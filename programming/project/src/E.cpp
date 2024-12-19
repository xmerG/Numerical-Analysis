#include<iostream>
#include<cmath>
#include<vector>
#include"Function.hpp"
#include"Curve_fitting.hpp"

using namespace std;

const double pi = acos(-1.);
const double c=sqrt(3);

class F1_x:public Function{
public:
    double operator()(double x) const{
        return sin(x)+x*cos(x);
    }
};

class F1_y:public Function{
public:
    double operator()(double x) const{
        return cos(x)-x*sin(x);
    }
};




class F2_x:public Function{
public:
    double operator()(double x) const{
        return c*cos(x);
    }
};

class F2_y:public Function{
public:
    double operator()(double x) const{
        return 2.0*(c*sin(x)+sqrt(c*abs(cos(x))))/3.0;
    }
};

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


//check if 2 splines are equal by neglecting small errors
bool is_almost_equal(vector<Polynomial> &p1, vector<Polynomial> &p2){
    if(p1.size()!=p2.size()){
        throw invalid_argument("The two vectors have different sizes.");
    }
    for(int i=0; i<p1.size(); ++i){
        if(p1[i].notequal(p2[i])){
            return false;
        }
    }
    return true;
}


void Fitting(const int &n, const double &a, const double &b, const Function &f1,
                const Function &f2, const boundaryType &btype, const string &filename){
    cubic_bspline_fit fit1(n,a,b,f1,f2,btype);
    vector<Polynomial> p1=fit1.getpol();
    fit1.print(filename);

    cubic_ppform_fit fit2(n,a,b,f1,f2,btype);
    vector<Polynomial> p2=fit2.getpol();
    fit2.print(filename);
    bool check2=is_almost_equal(p1,p2);
    cout<<"the consequence of 2 forms of spline is by neglecting small error "<<check2<<endl;
}

int main(){
    F1_x f1;
    F1_y f2;
    Fitting(10, 0.0, 6*pi, f1, f2,boundaryType::natural,"E_curve_r2.txt");
    Fitting(40, 0.0, 6*pi, f1, f2,boundaryType::natural,"E_curve_r2.txt");
    Fitting(160, 0.0, 6*pi, f1, f2,boundaryType::natural,"E_curve_r2.txt");
    Fitting(10, 0.0, 6*pi, f1, f2,boundaryType::complete,"E_curve_r2.txt");
    Fitting(40, 0.0, 6*pi, f1, f2,boundaryType::complete,"E_curve_r2.txt");
    Fitting(160, 0.0, 6*pi, f1, f2,boundaryType::complete,"E_curve_r2.txt");
    F2_x f3;
    F2_y f4;
    Fitting(10, -pi, pi, f3, f4,boundaryType::periodic,"E_curve_r1.txt");
    Fitting(40, -pi, pi, f3, f4,boundaryType::periodic,"E_curve_r1.txt");
    Fitting(160, -pi, pi, f3, f4,boundaryType::periodic,"E_curve_r1.txt");
    Fitting(10, -pi, pi, f3, f4,boundaryType::natural,"E_curve_r1.txt");
    Fitting(40, -pi, pi, f3, f4,boundaryType::natural,"E_curve_r1.txt");
    Fitting(160, -pi, pi, f3, f4,boundaryType::natural,"E_curve_r1.txt");
    Fitting(10, -pi, pi, f3, f4,boundaryType::complete,"E_curve_r1.txt");
    Fitting(40, -pi, pi, f3, f4,boundaryType::complete,"E_curve_r1.txt");
    Fitting(160, -pi, pi, f3, f4,boundaryType::complete,"E_curve_r1.txt");

    /*
    Fit(10, 0, 6*pi, f1, f2,"E_curve_r2.txt");
    Fit(40, 0, 6*pi, f1, f2,"E_curve_r2.txt");
    Fit(160, 0, 6*pi, f1, f2,"E_curve_r2.txt");
*/
    /*Fit(10, -pi, pi, f3, f4,"E_curve_r1.txt");
     
    Fit(40, -pi, pi, f3, f4,"E_curve_r1.txt");

    Fit(160, -pi, pi, f3, f4,"E_curve_r1.txt");*/
/*
    F3_x f5;
    F3_y f6;
    sphereFit fit1(40,0.0,2*pi,f5,f6);
    fit1.cubic_ppfit();
    fit1.print("E_spherecurve.txt");*/
    return 0;
}