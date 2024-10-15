#include"Interpolation.hpp"
#include"Polynomial.hpp"
#include"Function.hpp"
#include<iostream>
#include<cmath>
#include<vector>
#include <fstream>  // 引入文件流库
using namespace std;

int main(){
    ofstream outfile("D_output.txt");
    if(!outfile){
        cerr<<"Can't open the file"<<endl;
        return 1;
    }
    outfile<<"在0～13秒内，速度(feet/second)的函数大致为："<<endl;
    vector<double> time ={0.0, 3.0, 5.0, 8.0, 13.0};
    vector<double> displacement={0.0, 225.0, 383.0, 623.0, 993.0};
    vector<double> v={75.0, 77.0, 80.0, 74.0, 72.0};
    Hermiteinterpolation solve(time, displacement, v);
    Polynomial p=solve.getpolynomial();
    Polynomial dp=p.derivative();
    dp.print(outfile);
    
    
    cout<<"displament(t=10 s)= "<<p(10)<<endl;
    cout<<"velocity(t=10 s) = "<<dp(10)<<endl;
    /*for(double t=5.0; t<8.0; t+0.01){
        if(dp(t)> 81.0){
            cout<<"the car exceeded the speed limit of 81 feet per second at t= "<<t<<" s"<<endl;
            break;
        }
    }*/
    return 0;
}