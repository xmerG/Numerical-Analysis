#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP
#include<iostream>
#include<cmath>
#include<vector>
#include"Polynomial.hpp"
#include"Function.hpp"
using namespace std;


class Interpolation
{
    protected:
        vector<double> x, y;  //x 用于存储给定的插值点，y用于存储在给定点上的函数值
        int k=0; //k表示插值点个数
        vector<vector<double>> table; //记录差商表
    public:
        virtual void settable()=0; //构造差商表
        virtual Polynomial  getpolynomial()=0; //最终返回多项式
        vector<vector<double>> gettable(){
            settable();
            return table;
        }
        
        //打印差商表
        void print(){
            vector<vector<double>> table=gettable();
            for(int i=0; i<k; i++){
                for(int j=0; j<k+1;j++){
                    cout<<table[i][j];
                    if(j==k){
                        cout<<endl;
                    }
                }
            }
        }     
        virtual ~Interpolation()=default; 
};


//newton插值法
class Newtoninterpolation:public Interpolation{
    public:
        //初始数据是给定函数和插值点时
        Newtoninterpolation(const Function &f, const vector<double>_x){
            x=_x;
            k=x.size();
            for(int i=0; i<k; i++){
                y.push_back(f(x[i]));
            }
        }
        //初始数据是给定插值点和在插值点上的函数值
        Newtoninterpolation( const vector<double>_x, const vector<double>_y){
            x=_x;
            y=_y;
            k=x.size();
        }
        void settable(){
            //初始化差商表，元素都为0
            table.resize (k, vector<double>(k));
            //构造差商表
            for(int i=0; i<k; i++){
                table[i][0]=y[i];
            }   
            for (int j=1; j<k; j++){
                for(int i=j; i<k;i++){
                    table[i][j]=(table[i][j-1]-table[i-1][j-1])/(x[i]-x[i-j]);
                }
            } 
        }
        Polynomial getpolynomial(){
            vector<vector<double>> table=gettable();
            Polynomial p(vector<double>{table[k-1][k-1]});
            for(int i=k-2; i>=0; --i){
                Polynomial t(vector<double>{ -x[i], 1.0});
                p=p*t+Polynomial(vector<double>{table[i][i]});
            }
            return p;
        }
};

//hermite插值法
class Hermiteinterpolation:public Interpolation{
    private:
        vector<double> y1;                       //用于记录一阶导数
        int n;
    public:
        Hermiteinterpolation(const vector<double> &_x, const vector<double> &_y, 
                                                      const vector<double> &_y1): y1(_y1){
            x=_x;
            y=_y;
            k=x.size();
            n=2*k;
        }
        void settable(){
            //初始化差商表，元素都为0
            table.resize (n,vector<double>(n+1));
            //构造差商表
            for(int i=0; i<k; i++){
                table[2*i][0]=table[2*i+1][0]=x[i];
                table[2*i][1]=table[2*i+1][1]=y[i];
                table[2*i+1][2]=y1[i];
            }   
            for(int i=1; i<k; i++){
                table[2*i][2]=(table[2*i][1]-table[2*i-1][1])/(table[2*i][0]-table[2*i-1][0]);
            }
            for (int j=3; j<n+1; j++){
                for(int i=j-1; i<n; i++){
                    table[i][j]=(table[i][j-1]-table[i-1][j-1])/(table[i][0]-table[i+1-j][0]);
                }
            } 
        }
        Polynomial getpolynomial(){
            vector<vector<double>> table=gettable();
            Polynomial p(vector<double>{table[n-1][n]});
            for(int i=n-2; i>=0; --i){
                Polynomial t(vector<double>{ -table[i][0], 1.0});
                p=p*t+Polynomial(vector<double>{table[i][i+1]});
            }
            return p;
        }
};
#endif