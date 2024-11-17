#ifndef _BSPLINE_HPP_
#define _BSPLINE_HPP_
#include<iostream>
#include<vector>
#include<cmath>
#include"Function.hpp"
#include"Polynomial.hpp"
using namespace std;

enum class boundaryType{
    periodic,
    complete,
    specified,
    natural,
    not_a_knot
};

template<int degree>
class B_base{
protected:
    int n;//记录节点个数
    vector<double> knots; //记录supp_B上的节点
    vector<Polynomial> polys;  //记录每个区间上的多项式
public:
    B_base(){}
    
    vector<double> getknots(const int &index){
        vector<double> knots1;
        for(int j=index; j<index+degree+1){
            knots1.push_back(knots[j]);
        }
    }
    
    B_base(const vector<double> &_knots):knots{_knots}{
        if(degree==1){
            double delta1=knots[1]-knots[0]
            Polynomial p1(-knots[0]/delta1, 1.0/delta1);
            polys.push_back(p1);
            double delta2=knots[2]-knots[1];
            Polynomial p2(knots[2]/delta2, -1.0/delta2);
            polys.push_back(p2);
        }
        else{
            int n=degree;
            double delta1=knots[n]-knots[0];
            double delta2=knots[n+1]-knots[1];
            vector<double> knots1;
            vector<double> knots2;
            for(int j=0; j<n+1; ++j){
                knots1.push_back(knots[j]);
                knots2.push_back(knots[j+1]);
            }
            vector<Polynomial> p1=B_base<n-1>(knots1)*Polynomial(vector<double>{-knots[0]/delta1, 1.0/delta1});
            vector<Polynomial> p2=B_base<n-1>(knots2)*Polynomial(vector<double>{knots[n+1]/delta2, -1.0/delta2});
            polys.push_back(p1[0]);
            for(int j=1; j<n-1; ++j){
                polys.push_back(p1[j]+p2[j-1]);
            }
            polys.push_back(p2[n-1]);
        }
    }
    vector<Polynomial> getPolynomial(){
        return polys;
    }
    vector<Polynomial> operator*(const Polynomial &p) const{
        vector<Polynomial> q;
        for(int i=0; i<polys.size(); ++i)[
            q.push_back(polys[i]*p);
        ]
        return q;
    }
    //实现在节点处求值的功能,index传入节点的下标
    double B(const int &index) const{
        return polys[index](knots[i]);
    }

};

template<int degree>
class BSpline{
protected:
    int n;  //节点个数
    vector<double> knots;
    vector<Polynomial> pols;   
    vector<double> vals; //节点上的函数值,之后将求解得到的系数矩阵也储存在里面
public:
    void print(string &filename){

    }

    BSpline(){}
    BSpline(const vector<double> &_knots, const Function &F, btype=boundaryType::natural):knots{_knots}{
        n=_knots.size();  
        if(degree==1){
            for(int i=1; i<n-1; ++i){
                vals.push_back(F(knots[i]));
            }
            linear_Bspline();
        }
        //n=3的时候要多给6个节点,因此只在n-6个节点上有函数值，也就是要确定n-4个系数
        if(degree==3){
            vals.resize(n-4, 0.0);
            for(int i=1; i<n-5; ++i){
                vals[i]=F(knots[i+2]);
            }
        }
    }
    void linear_Bspline(){
        //一阶梯B样条插值点要多给两个，分别是第一个点和最后一个点
        for(int i=1; i<n-2; ++i){
            B_base<1> B1(knots[i-1], knots[i], knots[i+1]);
            B_base<1> B2(knots[i], knots[i+1], knots[i+2]); 
            pols.push_back(B1.getPolynomial()[1]*vals[i-1]+B2.getPolynomial()[0]*vals[i]);               
        }
    }

    void cubic_Bspline(boundaryType btype){
        vector<vector<double>> A; //记录系数矩阵
        A.resize(n,vector<double>(n-4, 0.0));
        for(int i=1; i<n-5; ++i){
            A[i][i-1]=pow((knots[i+3]-knots[i+2]), 2)/((knots[i+3]-knots[i])*(knots[i+3]-knots[i+1]));
            A[i][i]=((knots[i+2]-knots[i])*(knots[i+3]-knots[i+2])/(knots[i+3]-knots[i])+
                        (knots[i+4]-knots[i+2])*(knots[i+2]-knots[i+1])/(knots[i+4]-knots[i+1]))/(knots[i+3]-knots[i+1]);
            A[i][i+1]=pow(knots[i+2]-knots[i+1], 2)/((knots[i+4]-knots[i+1])*(knots[i+3]-knots[i+1]));
        }
        if(btype==boundaryType::complete, const Function &f){
            vals[0]=f.doubleDerivative(knots[3]);
            vals[n-5]=f.doubleDerivative(knots[n-4]);

        }
    }

};



#endif
