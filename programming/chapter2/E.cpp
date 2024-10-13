#include"Interpolation.hpp"
#include"Polynomial.hpp"
#include"Function.hpp"
#include<iostream>
#include<cmath>
#include<vector>
#include <fstream>  // 引入文件流库
using namespace std;

int main(){
    vector<double> x={0.0, 6.0, 10.0, 13.0, 17.0,20.0, 28.0};
    vector<double> y1={6.67, 17.3, 42.7, 37.3, 30.1, 29.3, 28.7};
    vector<double> y2={6.67, 16.1, 18.9, 15.0, 10.6, 9.44, 8.89};
    Newtoninterpolation f1(x, y1);
    Newtoninterpolation f2(x,y2);
    Polynomial p1=f1.getpolynomial();
    Polynomial p2=f2.getpolynomial();
    p1.print();
    p2.print();
    cout<<p1(43)<<endl;
    cout<<p2(43)<<endl;
    return 0;
}