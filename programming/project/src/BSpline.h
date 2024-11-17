#ifndef _BSPLINE_H_
#define _BSPLINE_H_
#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>
#include"Function.hpp"
#include"Polynomial.hpp"
#include <lapacke.h>
#include <nlohmann/json.hpp>
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
    int n; //记录节点个数
    vector<double> knots; //记录节点
    vector<Polynomial> pols;  //记录多项式
public:
    B_base(){};
    B_base(const vector<double> &_knots){
        knots=_knots;
        n=knots.size();
    }

    vector<Polynomial> getPolynomial(){
        return pols;
    }

    void getBase(){
        if(degree==1){
            double delta1=knots[1]-knots[0];
            Polynomial p1(vector<double>{-knots[0]/delta1,1.0/delta1});
            pols.push_back(p1);
            double delta2=knots[2]-knots[1];
            Polynomial p2(vector<double>{knots[2]/delta2,-1.0/delta2});
            pols.push_back(p2);
        }
        else{
            vector<vector<Polynomial>> u; //每个vecotr<Polynomial>存放B_i的多项式
            for(int i=1; i<degree+1; ++i){
                B_base<1> B1(vector<double>{knots[i-1], knots[i], knots[i+1]});
                B1.getBase();
                u.push_back(B1.getPolynomial());
            }
            for(int j=1; j<degree; ++j){
                vector<vector<Polynomial>> v;
                int m=degree+1-u.size(); //反映了当前多项式族所对应的B_base的阶
                for(int k=0; k<u.size()-1; ++k){
                    vector<Polynomial> v1;
                    double delta1=knots[k+1+m]-knots[k];
                    double delta2=knots[k+2+m]-knots[k+1];
                    v1.push_back(u[k][0]*Polynomial(vector<double>{-knots[k]/delta1, 1.0/delta1}));
                    for(int t=1; t<=m; ++t){
                        v1.push_back(u[k][t]*Polynomial(vector<double>{-knots[k]/delta1, 1.0/delta1})+
                                        u[k+1][t-1]*Polynomial(vector<double>{knots[k+2+m]/delta2, -1.0/delta2}));
                    }
                    v1.push_back(u[k+1][m]*Polynomial(vector<double>{knots[k+m+2]/delta2, -1.0/delta2}));
                    v.push_back(v1);
                }
                u=v;
            }
            pols=u[0];
        }
    }

    //定义Base和多项式的乘法
    vector<Polynomial> operator*(const Polynomial &p) const{
        vector<Polynomial> p1;
        for(int i=0; i<n-1; ++i){
            p1.push_back(p*pols[i]);
        }
        return p1;
    }
    //给定节点的指标i,构造support在knots[i-1]到knots[i+d]上的d阶B样条
    vector<double> setknots(const int &index, const int & d){
        vector<double> _knots;
        for(int i=index-1; i<d+index+1; ++i){
            _knots.push_back(knots[i]);
        }
        return _knots;
    }

    //在给定节点上求值
    double operator()(const int &index) const{
        if(0<index<n-1){
            return pols[index](knots[index]);
        }
        else{
            return 0.0;
        }
        
    }

    //在给定节点求一阶导数
/*    double derivative(const int &index) const{
        if(0<index<n-1){
            B_base<degree-1> B1(setknots(1,n-3));
            B_base<degree-1> B2(setknots(2,n-3));
            B1.getBase();
            B2.getBase();
            return degree*(B1(knots[index])/(knots[n-2]-knots[0])-B2(knots[index]/(knots[n-1]-knots[1])));
        }
        else{
            return 0.0;
        }
    } */

    void print() const {
        nlohmann::json j;
        j["knots"] = knots;
        nlohmann::json polynomials_json;
        for (const auto& poly : pols) {
            polynomials_json.push_back(poly.getcoefficents());  // 假设Polynomial类有getCoefficients()方法
        }
        j["polynomials"] = polynomials_json;
        j["degree"] = degree;
        j["num_knots"] = n;
        std::cout << j.dump(4) << std::endl;  // 格式化输出
    }

};


#endif