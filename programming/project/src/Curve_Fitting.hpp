#include<iostream>
#include<vector>
#include<cmath>
#include <nlohmann/json.hpp>
#include<fstream>
#include"Function.hpp"
#include"BSpline.hpp"
#include"Polynomial.hpp"
#include"ppForm.hpp"
using namespace std;
using json = nlohmann::json;


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
    Curve_Fit(vector<double> &_knots):knots{_knots}{}
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
    
        // 先检查文件是否为空
        std::ifstream file_check(filename);  // 用 ifstream 检查文件
        bool is_empty = file_check.peek() == std::ifstream::traits_type::eof();  // 判断文件是否为空
        file_check.close();  // 关闭检查用的文件流

        // 打开文件并以追加模式写入 JSON 数据
        std::ofstream file(filename, std::ios::app);  // 打开文件进行追加
        if (file.is_open()) {
            // 如果文件非空，则添加分隔符（换行符）
            if (!is_empty) {
                file << "\n";  // 可以根据需要使用其他分隔符
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
public:
    plane_curve_fit(vector<double> &knots, const vector<double> &_x_vals, const vector<double> &_y_vals):Curve_Fit(knots){
        x_vals=_x_vals;
        y_vals=_y_vals;
    }
    void cubic_ppform_fit(){
        cubic_ppForm s_x(knots, x_vals);
        polsX=s_x.returnPols();
        cubic_ppForm s_y(knots, y_vals);
        polsY=s_y.returnPols();
        convert();
    }

    void cubic_bspline_fit(const Function &f1, const Function &f2){
        BSpline<3> s_x(knots, f1, boundaryType::periodic);
        polsX=s_x.returnPols();
        BSpline<3> s_y(knots, f2, boundaryType::periodic);
        polsY=s_y.returnPols();
        convert();
    }

    void linear_ppform_fit(const Function &f1, const Function &f2){
        linear_ppForm s_x(knots, f1);
        polsX=s_x.returnPols();
        linear_ppForm s_y(knots, f2);
        polsY=s_y.returnPols();
        convert();
    }

    void linear_Bspline_fit(const Function &f1, const Function &f2){
        BSpline<1> s_x(knots, f1, boundaryType::periodic);
        polsX=s_x.returnPols();
        BSpline<1> s_y(knots, f2, boundaryType::periodic);
        polsY=s_y.returnPols();
        convert();
    }
};


vector<double> get_t(const int &N, const double &a, const double &b){
    vector<double> t(N+1);
    for(int i=0; i<N+1; ++i){
        t[i]=a+(b-a)*i/N;
    }
    return t;
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

void Fit(const int &N, const double &a, const double &b, const Function &f1, const Function &f2, const string &filename){
    vector<double> t=get_t(N,a,b);
    vector<double> x_val=getvals(t, f1);
    vector<double> y_val=getvals(t, f2);
    vector<vector<double>> points=getpoints(x_val,y_val);
    vector<double> knots=getknots(points);
    plane_curve_fit s1(knots, x_val, y_val);
    s1.cubic_ppform_fit();
    s1.print(filename);
}

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
};