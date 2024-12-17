#include<iostream>
#include<vector>
#include<cmath>
#include <nlohmann/json.hpp>
#include<fstream>
#include"Function.hpp"
#include"BSpline.h"
#include"Polynomial.hpp"
#include"ppForm.hpp"
using namespace std;
using json = nlohmann::json;

enum class knotsType {
    uniform,
    cumulate_chordal
};


double distance(const vector<double> &x, const vector<double> &y){
    if(x.size()!=y.size()){
        cerr<<"not same dimension points"<<endl;
        return -1;
    }
    else{
        double d=0.0;
        for(int i=0; i<x.size(); ++i){
            d+=pow(x[i]-y[i],2);
        }
        return sqrt(d);
    }
}
vector<double> getknots(const vector<vector<double>> &points) {
    double t=0.0;
    int n=points.size();
    vector<double> knots(n,0.0);
    for(int i=0; i<n-1; ++i){
        t+=distance(points[i],points[i+1]);
        knots[i+1]=t;
    }
    return knots;
}


class Curve_Fit{
protected:
    vector<double> knots; //knots
    vector<Polynomial> pols;  //polynomials x_1(t), y_1(t), x_2(t), y_2(t)...

public:
    Curve_Fit(){}
    Curve_Fit(vector<double> &_knots):knots{_knots}{}
    void print(const string& filename) {
        nlohmann::json j;

        j["knots"] = knots;
        vector<nlohmann::json> polynomials;
        for (const auto& poly : pols) {
            nlohmann::json poly_json;
            poly_json["coefficients"] = poly.getcoefficents();  
            polynomials.push_back(poly_json);
        }
        j["polynomials"] = polynomials;
    
        std::ifstream file_check(filename);  
        bool is_empty = file_check.peek() == std::ifstream::traits_type::eof(); 
        file_check.close(); 
        std::ofstream file(filename, std::ios::app);  // 打开文件进行追加
        if (file.is_open()) {
            // 如果文件非空，则添加分隔符（换行符）
            if (!is_empty) {
                file << "\n";  // 可以根据需要使用其他分隔符
            }
            file << j.dump(4);  
            file.close();
            cout << "Output appended to " << filename << endl;
        } 
        else {
            cerr << "Error opening file " << filename << endl;
        }
    }
};

class plane_curve_fit:public Curve_Fit{
protected:
    vector<double> x_vals;
    vector<double> y_vals;
    vector<Polynomial> polsX;
    vector<Polynomial> polsY;
    void convert(){
        for(int i=0; i<polsX.size(); ++i){
            pols.push_back(polsX[i]);
            pols.push_back(polsY[i]);
        }
    }

    vector<double> get_t(const int &N, const double &a, const double &b){
        vector<double> t(N+1);
        for(int i=0; i<N+1; ++i){
            t[i]=a+(b-a)*i/N;
        }
        return t;
    }

    void uniknots(const int &N, const double &a, const double &b, const Function &f1, const Function &f2){
        knots=get_t(N,a,b);
        x_vals=getvals(knots, f1);
        y_vals=getvals(knots, f2);
    }

    vector<double> getvals(const vector<double> &t, const Function &f){
        int n=t.size();
        vector<double> vals(n);
        for(int i=0; i<n; ++i){
            vals[i]=f(t[i]);
        }
        return vals;
    }

    vector<vector<double>> getpoints(const vector<double> &x, const vector<double> &y){
        int n=x.size();
        vector<vector<double>> points(n);
        for(int i=0; i<n; ++i){
            points[i].push_back(x[i]);
            points[i].push_back(y[i]);
        }
        return points;
    }
    void setknots(const int &N, const double &a, const double &b, const Function &f1, const Function &f2){
        vector<double> t=get_t(N,a,b);
        x_vals=getvals(t, f1);
        y_vals=getvals(t, f2);
        vector<vector<double>> points=getpoints(x_vals,y_vals);
        knots=getknots(points);
    }
    public:
        plane_curve_fit(){}
        /*plane_curve_fit(const int &n, const double &a, const double &b, const Function &f1, 
                        const Function &f2,const knotsType &t=knotsType::cumulate_chordal){
            if(t==knotsType::cumulate_chordal){
                setknots(n,a,b,f1,f2);
            }
            else{
                uniknots(n,a,b,f1,f2);
            }
        }
        void cubic_pp_fit(const Function &f1, const Function &f2,
                        const boundaryType &btype=boundaryType::periodic){
            cubic_ppForm s(knots,f1,btype);
            cubic_ppForm

            
        }*/
};

class cubic_bspline_fit:public plane_curve_fit{
public:
    cubic_bspline_fit(){}
    cubic_bspline_fit(const int &n, const double &a, const double &b, 
                    const Function &f1, const Function &f2, const boundaryType &btype=boundaryType::natural,
                    const knotsType &t=knotsType::cumulate_chordal){
        if(t==knotsType::uniform){
            uniknots(n, a, b, f1, f2);
        }
        else{
            setknots(n,a,b,f1,f2);
        }
        double c=knots[0]-knots[1];
        double d=knots[knots.size()-1]-knots[knots.size()-2];
        double e=knots[knots.size()-1];
        for(int i=1; i<=3;++i){
            knots.insert(knots.begin(),c*i);
            knots.push_back(e+1+d*i);
        }
        double x1=0.0;
        double x2=0.0;
        double y1=0.0;
        double y2=0.0;
        BSpline<3> b_x(knots, x_vals, btype);
        BSpline<3> b_y(knots, y_vals, btype);
        polsX=b_x.returnPols();
        polsY=b_y.returnPols();
        convert();
        for(int i=0; i<3; ++i){
            knots.erase(knots.begin());
            knots.pop_back();
        }
    } 
    vector<Polynomial> getpol(){
        return pols;
    }
};


class cubic_ppform_fit:public plane_curve_fit{
public:
    cubic_ppform_fit(const int &n, const double &a, const double &b, const Function &f1, const Function &f2,
                        const boundaryType &btype=boundaryType::natural,
                        const knotsType &t=knotsType::cumulate_chordal){
        if(t==knotsType::uniform){
            uniknots(n,a,b,f1,f2);
        }
        else{
            setknots(n,a,b,f1,f2);
        }
        cubic_ppForm b_x(knots, x_vals, btype);
        cubic_ppForm b_y(knots, y_vals, btype);
        polsX=b_x.returnPols();
        polsY=b_y.returnPols();
        convert();
    }
    vector<Polynomial> getpol(){
        return pols;
    }
};


//----------------------------------------------------------
/*
class sphereFit{
private:
    vector<vector<double>> points;
    vector<double> knots;
    vector<double> x_val;
    vector<double> y_val;
public:
    sphereFit(const int &N, const double &a, const double &b, const Function &f1, const Function &f2){
        vector<double> t=get_t(N,a,b);
        x_val=getvals(t, f1);
        y_val=getvals(t, f2);
        vector<vector<double>> points=getpoints(x_val,y_val);
        knots=getknots(points);
    }
    void cubic_ppfit(const boundaryType &btype=boundaryType::periodic){
        cubic_ppForm p1(knots,x_val,btype);
        vector<Polynomial> p_x=p1.returnPols();
        cubic_ppForm p2(knots,y_val,btype);
        vector<Polynomial> p_y=p2.returnPols();
        for(int i=0; i<p_x.size(); ++i){
            for(double j=knots[i];j<knots[i+1];j+=0.001){
                double a=p_x[i](j);
                double b=p_y[i](j);
                double c=pow(a,2)+pow(b,2);
                vector<double> p{2.0*a/(1+c), 2.0*b/(1+c), (1-c)/(1+c)};
                points.push_back(p);    
            }
        }
    }
    void print(const string& filename) {
        json j = points;  

        ifstream file_check(filename); 
        bool is_empty = file_check.peek() == std::ifstream::traits_type::eof();  // 判断文件是否为空
        file_check.close();  // 关闭检查用的文件流

        // 打开文件并以追加模式写入 JSON 数据
        std::ofstream file(filename, std::ios::app);  // 打开文件进行追加
        if (file.is_open()) {
            // 如果文件非空，则添加分隔符（换行符）
            if (!is_empty) {
                file << "\n";  // 你可以根据需要使用其他分隔符
            }

            // 将 JSON 数据写入文件，并格式化输出
            file << j.dump(4);  // 4 个空格缩进
            file.close();
            cout << "Output appended to " << filename << endl;
        } 
        else {
            cerr << "Error opening file " << filename << endl;
        }
    }
};*/