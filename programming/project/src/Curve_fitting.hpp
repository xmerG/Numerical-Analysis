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



class plane_curve_fit{
protected:
    vector<double> knots; //knots
    boundaryType btype;
    knotsType ktype;
    vector<double> x_vals;
    vector<double> y_vals;
    vector<Polynomial> polsX;
    vector<Polynomial> polsY;

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

    void clear(){
        x_vals.clear();
        x_vals.shrink_to_fit();
        y_vals.clear();
        y_vals.shrink_to_fit();
    }
    
public:
    plane_curve_fit(){}
    plane_curve_fit(const boundaryType &btype, const knotsType &ktype):btype{btype},ktype{ktype}{}
    void print(const string &filename) {
        ifstream inFile(filename);
        json jsonDataArray;
        // 如果文件存在且不为空，读取文件中的 JSON 数据
        if (inFile.is_open() && inFile.peek() != std::ifstream::traits_type::eof()) {
            inFile >> jsonDataArray;
            inFile.close();  // 读取完成后关闭输入文件
        } else {
            inFile.close();  // 如果文件为空或读取失败，关闭文件
        }

        // 如果文件为空或者没有读取到有效数据，初始化一个空的 JSON 数组
        if (jsonDataArray.is_null()) {
            jsonDataArray = json::array();
        }

        // 创建当前数据块
        json jsonData;
        jsonData["boundaryType"] = btype;
        jsonData["knotsType"] = ktype;
        json points;

        // 遍历 knots 和 polsX, polsY，生成数据点
        int n=knots.size();
        for (int i = 0; i < n- 1; ++i) {
            double l = knots[i + 1] - knots[i];
            for (double j = knots[i]; j <knots[i + 1]; j += l*0.015) {
                json point = { polsX[i](j), polsY[i](j) };  // 每个点是一个二维数组
                points.push_back(point);  // 将点添加到 "points" 数组中
            }
            json point={polsX[n-2](knots[n-1]), polsY[n-2](knots[n-1])};
            points.push_back(point);
        }
        jsonData["points"] = points;

        // 将新的数据块添加到 JSON 数组中
        jsonDataArray.push_back(jsonData);

        // 以追加模式打开文件
        ofstream outFile(filename, ios::out);
        if (outFile.is_open()) {
            outFile << jsonDataArray.dump(4) << endl;  // 格式化输出 (缩进4个空格)
            outFile.close();  // 保存并关闭文件
        } else {
            cout << "Failed to open file for writing" << endl;
        }
    }
};

class spherefit:public plane_curve_fit{
private:
    vector<vector<double>> points;
    void clear(){
        points.clear();
        points.shrink_to_fit();
    }
public:
    spherefit(const int &n, const double &a, const double &b, const Function &f1, const Function &f2, const knotsType &_ktype){
        ktype=_ktype;
        if(ktype==knotsType::cumulate_chordal){
            setknots(n,a,b,f1,f2);
        }
        else{
            uniknots(n,a,b,f1,f2);
        }
    }
    void cubic_ppFit(const boundaryType &_btype){
        btype=_btype;
        cubic_ppForm Sx(knots,x_vals);
        cubic_ppForm Sy(knots, y_vals);
        vector<Polynomial> p_x=Sx.returnPols();
        vector<Polynomial> p_y=Sy.returnPols();
        for(int i=0; i<p_x.size(); ++i){
            double a=knots[i+1]-knots[i];
            for(double j=knots[i];j<knots[i+1];j+=a*0.016){
                double a=p_x[i](j);
                double b=p_y[i](j);
                double c=pow(a,2)+pow(b,2);
                vector<double> p{2.0*a/(1+c), 2.0*b/(1+c), (1-c)/(1+c)};
                points.push_back(p);    
            }
        }
    }
    void print(const string &filename) {
        ifstream inFile(filename);
        json jsonDataArray;

        // 如果文件存在且不为空，读取文件中的 JSON 数据
        if (inFile.is_open() && inFile.peek() != std::ifstream::traits_type::eof()) {
            inFile >> jsonDataArray;
            inFile.close();  // 读取完成后关闭输入文件
        } else {
            inFile.close();  // 如果文件为空或读取失败，关闭文件
        }

    // 如果文件为空或者没有读取到有效数据，初始化一个空的 JSON 数组
        if (jsonDataArray.is_null()) {
            jsonDataArray = json::array();
        }

        // 创建当前数据块
        json jsonData;
        jsonData["boundaryType"] = btype;  
        jsonData["knotsType"] = ktype; 
    
        // 创建一个 JSON 数组来存储 points
        json pointsArray;

        for (const auto& point : points) {
            json pointJson;
            pointJson = {point[0], point[1], point[2]};
            pointsArray.push_back(pointJson);
        }

        // 将 points 数组添加到 jsonData 中
        jsonData["points"] = pointsArray;

        // 将新的数据块添加到 JSON 数组中
        jsonDataArray.push_back(jsonData);

        // 以追加模式打开文件
        ofstream outFile(filename, ios::out | ios::trunc);  // 使用 trunc 覆盖写入文件
        if (outFile.is_open()) {
            outFile << jsonDataArray.dump(4) << endl;  // 格式化输出 (缩进4个空格)
            outFile.close();  // 保存并关闭文件
        } else {
            cout << "Failed to open file for writing" << endl;
        }
        clear();
    }


};

class cubic_bspline_fit:public plane_curve_fit{
public:
    cubic_bspline_fit(){}
    cubic_bspline_fit(const int &n, const double &a, const double &b, 
                    const Function &f1, const Function &f2, const boundaryType &_btype=boundaryType::natural,
                    const knotsType &t=knotsType::cumulate_chordal){
        btype=_btype;
        ktype=t;
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
        for(int i=0; i<3; ++i){
            knots.erase(knots.begin());
            knots.pop_back();
        }
        clear();
    } 
};


class cubic_ppform_fit:public plane_curve_fit{
public:
    cubic_ppform_fit(const int &n, const double &a, const double &b, const Function &f1, const Function &f2,
                        const boundaryType &_btype=boundaryType::natural,
                        const knotsType &t=knotsType::cumulate_chordal){
        ktype=t;
        btype=_btype;
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
        clear();
    }
};


