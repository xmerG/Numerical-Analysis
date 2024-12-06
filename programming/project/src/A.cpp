#include<iostream>
#include<fstream>
#include"Function.hpp"
#include"ppForm.h"
using namespace std;


class Runge : public Function {
public:
    double operator() (double x) const {
        return 1/(1+25*x*x);
    }
};

class knots{
private:
    int n; //节点个数
    double a; //区间左端点
    double b; //区间右端点
    vector<double> knot;
public:
    knots(const int &n, const double &a, const double&b):n{n}, a{a}, b{b}{}
    vector<double> getknots(){
        for(int i=0; i<n+1; ++i){
            knot.push_back(a+(b-a)*i/n);
        }
    return knot;
    }

};
void test(const int &n){
    Runge f;
    knots x(n, -1.0, 1.0);
    vector<double> X=x.getknots();
    linear_ppForm l(X,f);
    l.print("output_A.txt");

    cubic_ppForm s(X, f);
    s.print("output_A.txt");

    cubic_ppForm s1(X, f,boundaryType::complete);
    s1.print("output_A.txt");

    cubic_ppForm s2(X, f,boundaryType::not_a_knot);
    s2.print("output_A.txt");

    cubic_ppForm s3(X, f,boundaryType::periodic);
    s3.print("output_A.txt");

    cubic_ppForm s4(X, f,boundaryType::specified);
    s4.print("output_A.txt");


}

int main(){
    test(6);
    test(11);
    test(21);
    test(41);
    test(81);
    return 0;
}