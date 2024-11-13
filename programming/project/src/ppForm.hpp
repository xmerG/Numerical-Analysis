#ifndef _PPFORM_H_
#define _PPFORM_H_
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include"Polynomial.hpp"
#include"Function.hpp"
#include"Interpolation.hpp"
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
        // 输出 knnots 和 pols 到 JSON 文件
    void print(const string& filename) {
        // 创建一个 JSON 对象
        nlohmann::json j;
        
        // 将 knots（节点）存储为 JSON 数组
        j["knots"] = knots;
        
        // 将 pols（多项式的系数）存储为 JSON 数组
        vector<nlohmann::json> polynomials;
        for (const auto& poly : pols) {
            nlohmann::json poly_json;
            poly_json["coefficients"] = poly.getcoefficents();  
            polynomials.push_back(poly_json);
        }
        j["polynomials"] = polynomials;
        
        // 打开文件并写入 JSON 数据
        std::ofstream file(filename);
        if (file.is_open()) {
            file << j.dump(4);  // 格式化输出，4个空格缩进
            file.close();
            cout << "Output written to " << filename << endl;
        }
        else {
            cerr << "Error opening file " << filename << endl;
        }
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
    vector<vector<double>> A;  //记录矩阵
    vector<double> b; //记录AX=b的b， 之后也会储存在knots上s(x)的导数
public:
    cubic_ppForm(){}
    cubic_ppForm(const vector<double> &_knots, const vector<double> &_vals, 
                    boundaryType _btype=boundaryType::natural):ppForm(_knots, _vals){
        cubic_ppForm();
        A.resize(n,vector<double>(n,0));
        btype=_btype;
    }
    cubic_ppForm(const vector<double> &_knots, const Function &F, 
                    boundaryType _btype=boundaryType::natural):ppForm(_knots, F){
        btype=_btype;
        double k1=divideddifference(knots[0],knots[1], vals[0], vals[1]); //第一个节点x_1处s的导数
        double kn=divideddifference(knots[n-2], knots[n-1], vals[n-2], vals[n-1]);  //第n-1个节点处s的导数
        prepare_for_Matrix();
        //完全样条
        if(btype==boundaryType::complete){
            A[0][0]=1.0;
            A[n-1][n-1]=1.0;
            b[0]=F.derivative(knots[0]);
            b[n-1]=F.derivative(knots[n-1]);
        }

        //自然样条
        else if(btype==boundaryType::natural){
            b[0]=3*k1;
            A[0][0]=2.0;
            A[n-1][n-1]=2.0;
            A[0][1]=1.0;
            A[n-1][n-2]=1.0;
            b[n-1]=3*kn;
        }

        //特定样条
        else if(btype==boundaryType::specified){
            double b1=-3.0*k1+(knots[1]-knots[0])*F.doubleDerivative(knots[0])/2.0;
            b[0]=b1;
            A[0][0]=2.0;
            A[n-1][n-1]=2.0;
            A[0][1]=1.0;
            A[n-1][n-2]=1.0;
            double bn=3.0*kn+F.doubleDerivative(knots[n-1])*(knots[n-1]-knots[n-2])/2.0;
            b[n-1]=bn;
        }

        //周期样条
        else if(btype==boundaryType::periodic){
            A[0][0]=1.0;
            A[0][n-1]=-1.0;
            double delta1=knots[1]-knots[0];
            double delta2=knots[n-1]-knots[n-2];
            A[n-1][0]=2*delta2;
            A[n-1][1]=delta2;
            A[n-1][n-2]=delta1;
            A[n-1][n-1]=2*delta1;
            b[0]=0.0;
            b[n-1]=3*k1*delta2+3*kn*delta1;
        }

        //not_a_knot样条
        else if(btype==boundaryType::not_a_knot){
            //x2处三阶导数存在
            double delta1=knots[1]-knots[0];
            double delta2=knots[2]-knots[1];
            A[0][0]=pow(delta2,2);
            A[0][1]=(knots[2]-knots[0]*(knots[2]+knots[0]-2*knots[1]));
            A[0][2]=pow(delta1, 2);
            b[0]=2*k1*pow(delta2,2)-2*divideddifference(knots[2], knots[1], vals[2], vals[1])*pow(delta1, 2);
            //x_{n-1}处三阶导数存在
            double delta3=knots[n-2]-knots[n-3];
            double delta4=knots[n-1]-knots[n-2];
            A[n-1][n-3]=pow(delta4, 2);
            A[n-1][n-2]=(knots[n-1]-knots[n-3])*(knots[n-1]+knots[n-3]-2*knots[n-2]);
            A[n-1][n-2]=pow(delta3, 2);
            b[n-1]=2*divideddifference(knots[n-2], knots[n-3], vals[n-2], vals[n-3])*delta4-2*kn*delta3;
            
        }
        getPiecePolys();
    }

    double divideddifference(const double &x1, const double &x2, const double &y1, const double &y2){
        return (y2-y1)/(x2-x1);
    }

    void prepare_for_Matrix(){
        A.resize(n,vector<double>(n,0.0));
        b.resize(n,0.0);
        for(int i=1; i<n-1; ++i){
            double miu=(knots[i]-knots[i-1])/(knots[i+1]-knots[i-1]);
            double lamda=1-miu;
            A[i][i]=2.0;
            A[i][i+1]=(miu);
            A[i][i-1]=lamda;  
            double dif1=divideddifference(knots[i],knots[i+1], vals[i], vals[i+1]);
            double dif2=divideddifference(knots[i-1], knots[i], vals[i-1], vals[i]);  
            b[i]=3*miu*dif1+3*lamda*dif2;
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
    void calculateDerivative(){
        vector<double> matrix=convert();
        vector<int> ipiv(n);
            // 调用 LAPACKE_dgesv 函数来求解 Ax = b,解的结果储存在b中
        int info = LAPACKE_dgesv(LAPACK_COL_MAJOR, n, 1, matrix.data(), n, ipiv.data(), b.data(), n);
    }
    void getPiecePolys(){
        calculateDerivative();
        for(int i=0; i<n-1; ++i){
            vector<double> m{b[i], b[i+1]}; //在[x_i,x_{i+1}]上的导数值
            vector<double> x{knots[i], knots[i+1]};  //节点
            vector<double> y{vals[i], vals[i+1]};
            Hermiteinterpolation h(x, y, m);
            pols.push_back(h.getpolynomial());
        }
    }


};


#endif