#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP
#include<iostream>
#include<cmath>
#include<vector>
using namespace std;

//创建多项式类
class Polynomial{
    private:
        vector<double> coefficients={0.0}; //记录多项式系数,按照升幂排列
        int n=0;  //记录最高次数
    public:
        Polynomial(){};
        Polynomial(vector<double> _coefficients):coefficients(_coefficients){
            n=coefficients.size()-1;
        }
        //返回多项式次数
        int Degree() const {
            return n;
        }
        //实现多项式的加法
        Polynomial operator+(const Polynomial &p1 ) const{
            int n1=max(n,p1.Degree());
            int n2=min(n, p1.Degree());
            vector<double> v(n1+1);
            for(int i=0; i<=n2; ++i){
                v[i]=coefficients[i]+p1.coefficients[i];
            }
            if(n1==n){
                for(int j=n2+1; j<=n1; ++j){
                    v[j]=coefficients[j];
                }                
            }
            else{
                for(int j=n2+1; j<=n1; ++j){
                    v[j]=p1.coefficients[j];
                }
            }
            return Polynomial(v);
        }

        //实现多项式减法
        Polynomial operator-()const{
            vector<double> v(n+1);
            for(int i=0; i<=n; ++i){
                v[i]=-coefficients[i];
            }
            return Polynomial(v);
        }
        Polynomial operator-(const Polynomial &p1){
            return *this+(-p1);
        }
        //实现多项式乘法
        Polynomial operator*(const Polynomial &p1)const{
            int m=n+p1.Degree();
            vector<double> v(m+1, 0.0);
            for(int i=0; i<=n; ++i){
                for(int j=0; j<=p1.Degree(); ++j){
                    v[i+j]+=coefficients[i]*p1.coefficients[j];
                }
            }  
            return Polynomial(v);
        }
        //多项式求值
        double operator()(double(x))const{
            double y=coefficients[n];
            for(int i=n-1; i>-1; --i){
                y*=x;
                y+=coefficients[i];
            }
            return y;
        }
        //多项式求导
        Polynomial derivative()const{
            if(n==0){
                Polynomial p1;
                return p1;
            }
            else{
                vector<double> d(n);
                for(int i=0; i<n; ++i){
                    d[i]=(i+1)*coefficients[i+1];
            }
            return Polynomial(d);
            }
        }
        //输出结果到文件
        void print(ostream &out=cout) const{
            for(int i=0; i<n+1; ++i){
                if(coefficients[i]>0.000001 && i!=0){  //去掉太小的项，美化输出
                    out<<"+"<<coefficients[i]<<"x^"<<i;
                }
                else if(abs(coefficients[i])<0.000001){}
                else{
                    out<<coefficients[i]<<"x^"<<i;
                }
            }
            out<<endl;
        }
        void printforpy(ostream &out=cout) const{
            for(int i=0; i<n+1;++i){
                out<<coefficients[i]<<" ";
            }
            out<<endl;
        }
};
#endif
