#include"Interpolation.hpp"
#include"Polynomial.hpp"
#include"Function.hpp"
#include<iostream>
#include<cmath>
#include<vector>
#include <fstream>  // 引入文件流库
using namespace std;

class F2:public Function{
    double operator()(double x) const{
        return 1/(1+25*x*x);
    }
};

const double _pi = acos(-1.0);

int main(){
    ofstream outfile("C_output.txt");
    if(!outfile){
        cerr<<"Can't open the file"<<endl;
        return 1;
    }
    outfile<<"原函数为 1/(1+25*x^2)"<<endl;

    //n=5
    vector<double>v1(5);
    for(int i=0; i<5; ++i){
        v1[i]=cos(_pi*(2*i+1)/10.0);
    }
    outfile<<"插值点x为："<<endl;
    for(double  i : v1){
        outfile<<i<<" ";
    }
    outfile<<endl;
    outfile<<"多项式P(x)"<<endl;
    Newtoninterpolation solve1(F2(), v1);
    Polynomial p1=solve1.getpolynomial();
    p1.print(outfile);

    //n=10
    outfile<<"插值点x"<<endl;
    vector<double> v2(10);
    for(int i=0; i<10; ++i){
        v2[i]=cos(_pi*(2*i+1)/20.0); 
    }
    for(double i : v2){
        outfile<<i<<" ";
    }
    outfile<<endl;
    outfile<<"多项式P(x)"<<endl;
    Newtoninterpolation solve2(F2(), v2);
    Polynomial p2=solve2.getpolynomial();
    p2.print(outfile);

    //n=15
    outfile<<"插值点x为"<<endl;
    vector<double> v3(15);
    for(int i=0; i<15; ++i){
        v3[i]=cos(_pi*(2*i+1)/30.0);
    }
    for(double i : v3){
        outfile<<i<<" ";
    }
    outfile<<endl;
    outfile<<"多项式P(x)"<<endl;
    Newtoninterpolation solve3(F2(), v3);
    Polynomial p3=solve3.getpolynomial();
    p3.print(outfile);

    //n=20
    outfile<<"插值点x为"<<endl;
    vector<double> v4(20);
    for(int i=0; i<20; ++i){
        v4[i]=cos(_pi*(2*i+1)/40.0);
    }
    for(double i: v4){
        outfile<<i<<" ";
    }
    outfile<<endl;
    outfile<<"多项式P(x)"<<endl;
    Newtoninterpolation solve4(F2(), v4);
    Polynomial p4=solve4.getpolynomial();
    p4.print(outfile);

    outfile.close();
    return 0;
}