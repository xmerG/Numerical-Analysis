#include<iostream>
#include<fstream>
#include"Function.hpp"
#include"ppForm.hpp"
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
        for(int i=0; i<n; ++i){
            knot.push_back(a+(b-a)*i/n);
        }
    return knot;
    }

};

int main(){
    Runge f;
    knots x(21, -1.0, 1.0);
    vector<double> X=x.getknots();
    cubic_ppForm s1(X,f);
    s1.print("output.txt");
    return 0;
}