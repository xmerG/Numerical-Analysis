#include"Interpolation.hpp"
#include"Polynomial.hpp"
#include"Function.hpp"
#include<iostream>
#include<cmath>
#include<vector>
#include <fstream>  // 引入文件流库
using namespace std;

class F1:public Function{
    public:
        double operator()(double x) const{
            return 1/(1+x*x);
    }
};


int main(){
    ofstream outfile("Boutput.txt");
    if(!outfile){
        cerr<<"Can't open the file"<<endl;
        return 1;
    }
    outfile<<"1/(1+x^2)"<<endl;

    //n=2的情况
    vector<double> v1;
    for(int i=0; i<=2;i++){
        v1.push_back(-5+static_cast<double> (10*i)/2.0);
    }
    outfile<<"插值点为x"<<endl;
    for(double x:v1){
        outfile<<x<<" ";
    }
    outfile<<endl;
    Newtoninterpolation solve1(F1(), v1);
    Polynomial p=solve1.getpolynomial();
    outfile<<"插值多项式p(x)"<<endl;
    p.print(outfile);

    //n=4的情况
    vector<double> v2;
    outfile<<"n=4时插值点x"<<endl;
    for(int i=0; i<=4;i++){
        v2.push_back(-5+static_cast<double> (10*i)/4.0);
        outfile<<v2[i]<<" ";
    }
    outfile<<endl;

    Newtoninterpolation solve2(F1(), v2);
    Polynomial p2=solve2.getpolynomial();
    outfile<<"插值多项式P(x)"<<endl;
    p2.print(outfile);

    //n=6的情况
    vector<double> v3;
    outfile<<"n=6 插值点x"<<endl;
    for(int i=0; i<=6;i++){
        v3.push_back(-5+static_cast<double> (10*i)/6.0);
        outfile<<v3[i]<<" ";
    }
    outfile<<endl;

    Newtoninterpolation solve3(F1(), v3);
    Polynomial p3=solve3.getpolynomial();
    outfile<<"插值多项式p(x)"<<endl;
    p3.print(outfile);

    //n=8的情况

    vector<double> v4;
    outfile<<"n=8 插值点x"<<endl;
    for(int i=0; i<=8; i++){
        v4.push_back(-5+static_cast<double> (10*i)/8.0);
        outfile<<v4[i]<<" ";
    }
    outfile<<endl;
    Newtoninterpolation solve4(F1(), v4);
    Polynomial p4=solve4.getpolynomial();
    outfile<<"插值多项式p(x)"<<endl;
    p4.print(outfile);
    outfile.close();
    return 0;
}
