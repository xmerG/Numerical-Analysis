#include<iostream>
#include<cmath>
#include<vector>
#include"Function.hpp"
#include"Curve_Fitting.hpp"

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



vector<double> get_t(const int &N, const double &a, const double &b){
    vector<double> t(N+1);
    for(int i=0; i<N+1; ++i){
        t[i]=a+(b-a)*i/N;
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
        int n=x.size();
        vector<vector<double>> points(n);
        for(int i=0; i<n; ++i){
            points[i].push_back(x[i]);
            points[i].push_back(y[i]);
        }
        return points;
}

void Fit(const int &N, const double &a, const double &b, const Function &f1, const Function &f2, const string &filename){
    vector<double> t=get_t(N,a,b);
    vector<double> x_val=getvals(t, f1);
    vector<double> y_val=getvals(t, f2);
    vector<vector<double>> points=getpoints(x_val,y_val);
    vector<double> knots=getknots(points);
    plane_curve_fit s1(knots, x_val, y_val);
    s1.cubic_ppform_fit();
    s1.print(filename);
}

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


int main(){
    F1_x f1;
    F1_y f2;
    Fit(10, 0, 6*pi, f1, f2,"E_curve_r2.txt");
    Fit(40, 0, 6*pi, f1, f2,"E_curve_r2.txt");
    Fit(160, 0, 6*pi, f1, f2,"E_curve_r2.txt");


    F2_x f3;
    F2_y f4;
    Fit(10, -pi, pi, f3, f4,"E_curve_r1.txt");
     
    Fit(40, -pi, pi, f3, f4,"E_curve_r1.txt");

    Fit(160, -pi, pi, f3, f4,"E_curve_r1.txt");


    return 0;
}