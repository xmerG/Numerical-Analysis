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
#include"boundaryType.hpp"
using namespace std;

using json = nlohmann::json;


template<int degree>
class B_base{
protected:
    int n; //记录节点 个数
    vector<double> knots; //记录节点
    vector<Polynomial> pols;  //记录分段多项式
public:
    B_base(){};
    B_base(const vector<double> &_knots){
        knots=_knots;
        n=knots.size();
        if(n!=degree+2){
            cerr<<"Error:节点和阶数不匹配"<<endl;
            exit(EXIT_FAILURE);
        }
    }

    //给定节点的指标i,构造support在knots[i-1]到knots[i+d]上的d阶B样条
    vector<double> setknots(const int &index, const int & d) const{
        vector<double> _knots;
        for(int i=index-1; i<d+index+1; ++i){
            _knots.push_back(knots[i]);
        }
        return _knots;
    }

    vector<Polynomial> getPolynomial() const{
        return pols;
    }

    //构造分段多项式
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
    /*vector<Polynomial> operator*(const Polynomial &p) const{
        vector<Polynomial> p1;
        for(int i=0; i<n-1; ++i){
            p1.push_back(p*pols[i]);
        }
        return p1;
    }*/


    //在给定节点上求值
    double operator()(const int &index) const{
        if(0<index<n-1){
            return pols[index](knots[index]);
        }
        else{
            return 0.0;
        }
        
    }

    //在给定节点求一阶导数,如果只求导，不要对B_base<degree>执行getbase()
    double derivative(const int &index) const{
        if(0<index<n-1){
            B_base<degree-1> B1(setknots(1,n-3));
            B_base<degree-1> B2(setknots(2,n-3));
            B1.getBase();
            B2.getBase();
            return degree*(B1(knots[index])/(knots[n-2]-knots[0])-B2(knots[index])/(knots[n-1]-knots[1]));
        }
        else{
            return 0.0;
        }
    } 
    //3rd-derivative
    double left_thirdDerivatiev(const double &x) const{
        for(int i=0; i<=knots.size()-2; ++i){
            if(knots[i]<x && x<=knots[i+1]){
                return pols[i].thirdDerivative(x);
            }
        }
        return 0;
    }

    double right_thirdDerivative(const double &x) const{
        for(int i=0; i<knots.size()-2; ++i){
            if(knots[i]<=x&& x<knots[i+1]){
                return pols[i].thirdDerivative(x);
            }
        }
    }



};

//实现Bspline
template<int degree>
class BSpline{
private:
    vector<double> knots; //记录节点 要多记录2*degree个节点
    vector<vector<double>> A; //系数矩阵
    vector<double> b; //记录节点上的函数值，最终会将基函数的系数储存在b中
    vector<vector<Polynomial>> bases; //记录基函数
    vector<Polynomial> pols;  //记录多项式
    int n;  //the number of bases
    boundaryType btype=boundaryType::non;

    void prepare(){
        for(int i=0; i<n; ++i){
            vector<double> subknots(knots.begin()+i,knots.begin()+i+degree+2);
            B_base<degree> base(subknots);
            base.getBase();
            bases.push_back(base.getPolynomial());
        }
    }

    void getMatrix(){
        if(degree==3){
            A.resize(n,vector<double>(n,0.0));
            for(int i=1; i<n-1; ++i){
                A[i][i-1]=bases[i-1][3](knots[i+2]);
                A[i][i]=bases[i][2](knots[i+2]);
                A[i][i+1]=bases[i+1][1](knots[i+2]);
            }
        }
    }

    vector<double> convert(){
        vector<double> a;
        for(int i=0; i<n; ++i){
            for(int j=0; j<n; ++j){
                a.push_back(A[j][i]);
            }
        }
        return a;

    }

    void calculateCoefficient(){
        vector<double> matrix=convert();
        vector<int> ipiv(n);
            // 调用 LAPACKE_dgesv 函数来求解 Ax = b,解的结果储存在b中
        int info = LAPACKE_dgesv(LAPACK_COL_MAJOR, n, 1, matrix.data(), n, ipiv.data(), b.data(), n);
    }

    void getpiecewisePoly(){
        calculateCoefficient();
        for(int i=0; i<n-degree; ++i){
            Polynomial p;
            p=bases[i][3]*Polynomial(vector<double>{b[i]})+bases[i+1][2]*Polynomial(vector<double> {b[i+1]});
            Polynomial p2;
            p2=bases[i+2][1]*Polynomial(vector<double> {b[i+2]})+bases[i+3][0]*Polynomial(vector<double> {b[i+3]});
            Polynomial p3;
            p3=p+p2;
            pols.push_back(p3);
        }
    }

    void fit(const double &a1=0.0, const double &a2=0.0){
        if(degree==1){
            for(int i=0;i<bases.size()-1; ++i){
                pols.push_back(bases[i][1]*Polynomial(vector<double>{b[i]})+bases[i+1][0]*Polynomial(vector<double>{b[i+1]}));
            }
        }
        if(degree==3){
            getMatrix();
            // complete spline
            if(btype==boundaryType::complete){
                b[0]=a1;
                b[n-1]=a2;
                A[0][0]=bases[0][3].derivative(knots[3]);
                A[0][1]=bases[1][2].derivative(knots[3]);
                A[0][2]=bases[2][1].derivative(knots[3]);
                A[n-1][n-3]=bases[n-3][3].derivative(knots[n]);
                A[n-1][n-2]=bases[n-2][2].derivative(knots[n]);
                A[n-1][n-1]=bases[n-1][1].derivative(knots[n]);
            }

            //natural spline
            else if(btype==boundaryType::natural){
                double delta1=knots[4]-knots[1];
                double delta2=knots[4]-knots[2];
                double delta3=knots[5]-knots[2];
                A[0][0]=6.0/(delta1*delta2);
                A[0][1]=(-6.0/delta1-6.0/delta3)/delta2;
                A[0][2]=6.0/(delta3*delta2);
                delta1=knots[n+1]-knots[n-2];
                delta2=knots[n+1]-knots[n-1];
                delta3=knots[n+2]-knots[n-1];
                A[n-1][n-3]=6.0/(delta1*delta2);
                A[n-1][n-2]=(-6.0/delta1-6.0/delta3)/delta2;
                A[n-1][n-1]=6.0/(delta2*delta3);
            }

            //specified spline
            else if(btype==boundaryType::specified){
                double delta1=knots[4]-knots[1];
                double delta2=knots[4]-knots[2];
                double delta3=knots[5]-knots[2];
                A[0][0]=6.0/(delta1*delta2);
                A[0][1]=(-6.0/delta1-6.0/delta3)/delta2;
                A[0][2]=6.0/(delta3*delta2);
                b[0]=a1;
                delta1=knots[n+1]-knots[n-2];
                delta2=knots[n+1]-knots[n-1];
                delta3=knots[n+2]-knots[n-1];
                A[n-1][n-3]=6.0/(delta1*delta2);
                A[n-1][n-2]=(-6.0/delta1-6.0/delta3)/delta2;
                A[n-1][n-1]=6.0/(delta2*delta3);
                b[n-1]=a2;
            }

            //periodic spline
            else if(btype==boundaryType::periodic){
                //repace s(b)=f(b) with s(b)=f(a)
                b[n-2]=b[1];
                // second derivative of a and b is equal
                double delta1=knots[4]-knots[1];
                double delta2=knots[4]-knots[2];
                double delta3=knots[5]-knots[2];
                A[n-1][0]=6.0/(delta1*delta2);
                A[n-1][1]=(-6.0/delta1-6.0/delta3)/delta2;
                A[n-1][2]=6.0/(delta3*delta2);
                delta1=knots[n+1]-knots[n-2];
                delta2=knots[n+1]-knots[n-1];
                delta3=knots[n+2]-knots[n-1];
                A[n-1][n-3]=-6.0/(delta1*delta2);
                A[n-1][n-2]=(6.0/delta1+6.0/delta3)/delta2;
                A[n-1][n-1]=-6.0/(delta2*delta3);
                //first oreder derivative of a and b is equal
                A[0][0]=bases[0][3].derivative(knots[3]);
                A[0][1]=bases[1][2].derivative(knots[3]);
                A[0][2]=bases[2][1].derivative(knots[3]);
                A[0][n-3]=-bases[n-3][3].derivative(knots[n]);
                A[0][n-2]=-bases[n-2][2].derivative(knots[n]);
                A[0][n-1]=-bases[n-1][1].derivative(knots[n]);
            }

            // not-a-knot spline
            else if(btype==boundaryType::not_a_knot){
                //left_3rd derivative-right_3rd_derivative=0
                double x=knots[degree+1];
                A[0][0]=bases[0][3].thirdDerivative(x);
                A[0][1]=bases[1][2].thirdDerivative(x)-bases[1][3].thirdDerivative(x);
                A[0][2]=bases[2][1].thirdDerivative(x)-bases[2][2].thirdDerivative(x);
                A[0][3]=bases[3][0].thirdDerivative(x)-bases[3][1].thirdDerivative(x);
                A[0][4]=-bases[4][0].thirdDerivative(x);
                //right_3rd_derivative-left_3rd_derivative=0
                double y=knots[n-1];
                A[n-1][n-1]=bases[n-1][0].thirdDerivative(y);
                A[n-1][n-2]=bases[n-2][1].thirdDerivative(y)-bases[n-2][0].thirdDerivative(y);
                A[n-1][n-3]=bases[n-3][2].thirdDerivative(y)-bases[n-3][1].thirdDerivative(y);
                A[n-1][n-4]=bases[n-4][3].thirdDerivative(y)-bases[n-4][2].thirdDerivative(y);
                A[n-1][n-5]=-bases[n-5][3].thirdDerivative(y);
            }
            getpiecewisePoly();
        }
    }
    void clear(){
        A.clear();
        A.shrink_to_fit();
    }

public:
    BSpline(){}
    BSpline(const vector<double> &_knots, const vector<double> &vals,const boundaryType &_btype=boundaryType::non,
                const double &a1=0.0, const double &a2=0.0):knots{_knots},btype{_btype}{
        n=knots.size()-degree-1;
        b.resize(n,0.0);
        prepare();
        if(degree==1){
            b=vals;
        }
        else if(degree==3){
            for(int i=1; i<n-1; ++i){
                b[i]=vals[i-1];
            }
            fit(a1,a2);
        }
        clear();
    }
    BSpline(const vector<double> &_knots, const Function &F, const boundaryType &_btype=boundaryType::non):knots{_knots}{
        n=knots.size()-degree-1;
        b.resize(n,0.0);
        btype=_btype;
        prepare();
        if(degree==1){
            for(int i=0; i<n; ++i){
                b[i]=F(knots[i+1]);
            }
            fit();
        }
        if(degree==3){
            for(int i=1; i<n-1; ++i){
                b[i]=F(knots[i+degree-1]);
            }

            // complete spline
            if(btype==boundaryType::complete){
                fit(F.derivative(knots[3]),F.derivative(knots[n]));
            }


            //specified spline
            else if(btype==boundaryType::specified){
                fit(F.doubleDerivative(knots[3]), F.doubleDerivative(knots[n]));
            }
            else{
                fit();
            }
        }
        clear();


    }

    BSpline(const vector<vector<Polynomial>> &base, const vector<double> &coef){
        if(base.size()!=coef.size()){
            cerr<<"not a Bspline"<<endl;
            return;
        }
        if(base[0].size()!=degree+2){
            cerr<<"the degree is wrong!"<<endl;
            return;
        }
        else{
            b=coef;
            bases=base;
        }
        n=coef.size();
        for(int i=0; i<n ;++i){
            Polynomial p;
            for(int j=0; j<=degree; ++j){
                p=p+Polynomial(vector<double> {b[i]})*bases[i+j][degree-j];
            }
            pols.push_back(p);
        }
    }

    //calculate the value of arbitrary BSpline if we already know the expression
    double calculateValue(const double &x){
        double y=0.0;
        //find the interval containing x
        for(int i=degree; i<=n-1; ++i){
            if(knots[i]<=x && x<=knots[i+1]){
                y=pols[i-degree](x);
                return y;
                //for(int j=degree; j>=0; --j){
                    //y+=bases[i-j][j](x)*b[i-j];
                //}
            }
        }
        return y;
    }


    void print(const string& filename) {

        nlohmann::json j;
        j["boundary_condition"] = btype;
        vector<double> newknots(knots.begin() + degree, knots.end() - degree); 
        j["knots"] = newknots;

        // 将 pols（多项式的系数）存储为 JSON 数组
        vector<nlohmann::json> polynomials;
        for (const auto& poly : pols) {
            nlohmann::json poly_json;
            poly_json["coefficients"] = poly.getcoefficents();  
            polynomials.push_back(poly_json);
        }
        j["polynomials"] = polynomials;

    // 检查文件是否为空
        std::ifstream file_check(filename);  // 用 ifstream 检查文件
        bool is_empty = file_check.peek() == std::ifstream::traits_type::eof();  // 判断文件是否为空
        file_check.close();  // 关闭检查用的文件流

        nlohmann::json jsonDataArray;

        if (!is_empty) {
            std::ifstream inFile(filename);  // 打开文件进行读取
            inFile >> jsonDataArray;  // 读取 JSON 数据
            inFile.close();  // 关闭读取文件流
        }

        jsonDataArray.push_back(j);

    // 打开文件进行写入（覆盖文件内容）
        std::ofstream outFile(filename, std::ios::out | std::ios::trunc);  // 打开文件，清空内容
        if (outFile.is_open()) {
            // 将修改后的 JSON 数组写回文件，并格式化输出
            outFile << jsonDataArray.dump(4);  // 4 个空格缩进
            outFile.close();
        } 
        else {
            cerr << "Error opening file " << filename << endl;
        }
    }


    //get polynomials
    vector<Polynomial> returnPols(){
        return pols;
    }

};


#endif