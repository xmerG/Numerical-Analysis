#ifndef _PPFORM_H_
#define _PPFORM_H_
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include"Polynomial.hpp"
#include"Function.hpp"
using namespace std;

enum class boundaryType{
    periodic,
    complete,
    specified,
    natural,
    not_a_knot
};

class ppForm{
protected:
    int n;                //记录节点个数
    vector<double> knots; //记录给定的节点
    vector<double> vals;  //记录节点上的函数值
    vector<Polynomial> pols; //每一个多项式代表了在对应编号区间上得到的插值多项式
public:
    ppForm(){}
    //节点上的函数值需要自己计算
    ppForm(const vector<double> &_knots, const Function &F):knots{_knots}{
        n=knots.size();
        for(int i=0; i<n; ++i){
            vals.push_back(F(knots[i]));
        }
    }
    //节点的函数值已经给出
    ppForm(const vector<double> &_knots, const vector<double> &_vals):knots{_knots}, vals{_vals}{
        n=knots.size();
    }
};

class linear_ppForm:public ppForm{
public:
    linear_ppForm(){}
    linear_ppForm(const vector<double> &_knots, const Function &F):ppForm(_knots, F){
        linear_ppForm();
        fit();
    }
    linear_ppForm(const vector<double> &_knots, const vector<double> &_vals):ppForm(_knots,_vals){
        linear_ppForm();
        fit();
    }
    //计算每个区间上的多项式
    void fit(){
        for(int i=0; i<n-1; ++i){
            //一次函数的斜率
            double k=(vals[i+1]-vals[i])/(knots[i+1]-knots[i]);
            vector<double> coeff{vals[i]-knots[i]*k, k};
            Polynomial pi(coeff);
            pols.push_back(pi);
        }
    }
};

class cubic_ppForm:public ppForm{
private:
    //记录边界条件
    boundaryType btype;
    vector<double> b; //记录AX=b的b
    vector<vector<double>> matrix;  //记录关于一阶导数m_i的矩阵
    vector<double> diagonal;  //记录matrix的主对角元，已知除了第一个和最后一个元素，其余都是2,因此这个只存放3个元素
    vector<double> l; //记录matrix的下副对角线
    vector<double> u; //记录matrix的上副对角线
public:
    cubic_ppForm(){}
    cubic_ppForm(const vector<double> &_knots, const vector<double> &_vals, 
                    boundaryType _btype=boundaryType::natural){
        cubic_ppForm();
        btype=_btype;
    }
    cubic_ppForm(const vector<double> &_knots, const Function &F, 
                    boundaryType _btype=boundaryType::natural){
        btype=_btype;
        double k1=divideddifference(knots[0],knots[1], vals[0], vals[1]); //第一个节点x_1处s的导数
        double kn=divideddifference(knots[n-2], knots[n-1], vals[n-2], vals[n-1]);  //第n-1个节点处s的导数
        //完全样条
        if(btype==boundaryType::complete){
            diagonal.push_back(1.0);
            b.push_back(F.derivative(knots[0]));
            prepare_for_Matrix();
            diagonal.push_back(1.0);
            b.push_back(F.derivative(knots[n-1]));
        }

        //自然样条
        if(btype==boundaryType::natural){
            b.push_back(k1);
            diagonal.push_back(2.0);
            u.push_back(1.0);
            prepare_for_Matrix();
            diagonal.push_back(2.0);
            l.push_back(1.0);
            b.push_back(kn);
        }

        //特定样条
        if(btype==boundaryType::specified){
            double b1=-3.0*k1+(knots[1]-knots[0])*F.doubleDerivative(knots[0])/2.0;
            b.push_back(b1);
            diagonal.push_back(2.0);
            u.push_back(1.0);
            prepare_for_Matrix();
            diagonal.push_back(2.0);
            l.push_back(1.0);
            double bn=3.0*kn+F.doubleDerivative(knots[n-1])*(knots[n-1]-knots[n-2])/2.0;
            b.push_back(bn);
        }

        //周期样条
        if(btype==boundaryType::periodic){
            diagonal.push_back(1.0);
            b.push_back(0.0);
            ???
            prepare_for_Matrix();
            ???
        }

        //not_a_knot样条
        if(btype==boundaryType::not_a_knot){
            u.push_back(-pow(knots[1]-knots[0], 2));
            l.push_back(pow(knots[2]-knots[1], 2));
            diagonal.push_back((knots[0]+knots[2]-2*knots[1])*(knots[2]-knots[0]));
            double k2=divideddifference(knots[2], knots[1], vals[2], vals[1]);
            b.push_back(2*k1*pow(knots[2]-knots[1], 2)-2*k2*pow(knots[0]-knots[1], 2));
            prepare_for_Matrix();
            u.push_back(pow(knots[n-1]-knots[n-2], 2));
            l.push_back(pow(knots[n-3]-knots[n-2], 2));
            diagonal.push_back((knots[n-1]+knots[n-3]-2*knots[n-2])*(knots[n-1]-knots[n-3]));
            double k=divideddifference(knots[???])
        }
    }

    double divideddifference(const double &x1, const double &x2, const double &y1, const double &y2){
        return (y2-y1)/(x2-x1);
    }

    void prepare_for_Matrix(){
        for(int i=1; i<n-1; ++i){
            diagonal.push_back(2.0);
            double miu=(knots[i]-knots[i-1])/(knots[i+1]-knots[i-1]);
            double lamda=1-miu;
            u.push_back(miu);
            l.push_back(lamda);
            double dif1=divideddifference(knots[i],knots[i+1], vals[i], vals[i+1]);
            double dif2=divideddifference(knots[i-1], knots[i], vals[i-1], vals[i]);
            b.push_back(3*miu*dif1+3*lamda*dif2);            
        }
    }
};


#endif