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
                const Function &f2, const string &filename){
    cubic_bspline_fit fit1(n,a,b,f1,f2,boundaryType::complete);
    fit1.print(filename);

    cubic_ppform_fit fit2(n,a,b,f1,f2,boundaryType::complete);
    fit2.print(filename);

    cubic_ppform_fit s1(n,a,b,f1,f2,boundaryType::complete,knotsType::uniform);
    s1.print(filename);
//------------
    cubic_bspline_fit fit3(n,a,b,f1,f2,boundaryType::periodic);
    fit3.print(filename);

    cubic_ppform_fit fit4(n,a,b,f1,f2,boundaryType::periodic);
    fit4.print(filename);

    cubic_ppform_fit s3(n,a,b,f1,f2,boundaryType::periodic,knotsType::uniform);
    s3.print(filename);
//--------------------------------------
    cubic_bspline_fit fit5(n,a,b,f1,f2,boundaryType::natural);
    fit5.print(filename);

    cubic_ppform_fit fit6(n,a,b,f1,f2,boundaryType::natural);
    fit6.print(filename);

    cubic_ppform_fit s6(n,a,b,f1,f2,boundaryType::natural,knotsType::uniform);
    s6.print(filename);

    cubic_bspline_fit fit7(n,a,b,f1,f2,boundaryType::not_a_knot);
    fit7.print(filename);

    cubic_ppform_fit fit8(n,a,b,f1,f2,boundaryType::not_a_knot);
    fit8.print(filename);

    cubic_ppform_fit s8(n,a,b,f1,f2,boundaryType::not_a_knot,knotsType::uniform);
    s8.print(filename);

}
void sphereFit(const int &n, const double &a, const double &b, const Function &f1,
                const Function &f2, const string &filename){
    spherefit fit1(n,a,b,f1,f2,knotsType::cumulate_chordal);
    fit1.cubic_ppFit(boundaryType::periodic);
    fit1.print(filename);

    spherefit fit2(n,a,b,f1,f2,knotsType::uniform);
    fit2.cubic_ppFit(boundaryType::periodic);
    fit2.print(filename);
//-----------------------------
    fit1.cubic_ppFit(boundaryType::natural);
    fit1.print(filename);

    fit2.cubic_ppFit(boundaryType::natural);
    fit2.print(filename);
//---------------
    fit1.cubic_ppFit(boundaryType::complete);
    fit1.print(filename);

    fit2.cubic_ppFit(boundaryType::complete);
    fit2.print(filename);
//------------------------
    fit1.cubic_ppFit(boundaryType::not_a_knot);
    fit1.print(filename);

    fit2.cubic_ppFit(boundaryType::not_a_knot);
    fit2.print(filename);

}

int main(){
    F1_x f1;
    F1_y f2;
    Fitting(10, 0.0, 6*pi, f1, f2,"E_curve2.json");
    Fitting(40, 0.0, 6*pi, f1, f2,"E_curve2.json");
    Fitting(160, 0.0, 6*pi, f1, f2,"E_curve2.json");
    F2_x f3;
    F2_y f4;
    Fitting(10, -pi, pi, f3, f4,"E_curve1.json");
    Fitting(40, -pi, pi, f3, f4,"E_curve1.json");
    Fitting(160, -pi, pi, f3, f4,"E_curve1.json");

    F3_x f5;
    F3_y f6;
    sphereFit(10,0.0, 2*pi, f5, f6, "sphere_curve.json");
    sphereFit(40,0.0, 2*pi, f5, f6, "sphere_curve.json");
    sphereFit(160,0.0, 2*pi, f5, f6, "sphere_curve.json");
    return 0;
}