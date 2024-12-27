#include<iostream>
#include<fstream>
#include"Function.hpp"
#include"ppForm.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;


class Runge : public Function {
public:
    double operator() (double x) const {
        return 1/(1+25*x*x);
    }
};

class knots{
private:
    int n; //节点个数
    double a; //区间左端点
    double b; //区间右端点
    vector<double> knot;
    vector<double> midpoints;
public:
    knots(const int &n, const double &a, const double&b):n{n}, a{a}, b{b}{}
    vector<double> getknots(){
        for(int i=0; i<n+1; ++i){
            knot.push_back(a+(b-a)*i/n);
        }
    return knot;
    }
    vector<double> Midpoints(){
        for(int i=0; i<n; ++i){
            midpoints.push_back((knot[i]+knot[i+1])/2);
        }
        return midpoints;
    }

};

void error(const ppForm &s, const Function &f, vector<double> &mid){
    vector<double> e;
    for(int i=0; i<mid.size(); ++i){
        e.push_back(s.calculate(mid[i])-f(mid[i]));
    }
    json error_data;
    error_data["mid"] = mid;
    error_data["error"] = e;

    ifstream inFile("error_A.json");
    json jsonDataArray;

    if (inFile.is_open() && inFile.peek() != ifstream::traits_type::eof()) {
        inFile >> jsonDataArray;
    }
    inFile.close();

    if (jsonDataArray.is_null()) {
        jsonDataArray = json::array();
    }

    jsonDataArray.push_back(error_data);

    ofstream outFile("error_A.json", ios::out);
    if (outFile.is_open()) {
        outFile << jsonDataArray.dump(4) << endl;
        outFile.close();
    } else {
        cerr << "Failed to open file for writing" << endl;
    }
}


void test(const int &n){
    Runge f;
    knots x(n, -1.0, 1.0);
    vector<double> X=x.getknots();
    vector<double> mid=x.Midpoints();
    linear_ppForm l(X,f);
    l.print("output_A.json");
    error(l,f,mid);


    cubic_ppForm s(X, f);
    s.print("output_A.json");
    error(s,f,mid);

    cubic_ppForm s1(X, f,boundaryType::complete);
    s1.print("output_A.json");
    error(s1,f,mid);

    cubic_ppForm s2(X, f,boundaryType::not_a_knot);
    s2.print("output_A.json");
    error(s2,f,mid);

    cubic_ppForm s3(X, f,boundaryType::periodic);
    s3.print("output_A.json");
    error(s3,f,mid);

    cubic_ppForm s4(X, f,boundaryType::specified);
    s4.print("output_A.json");
    error(s4,f,mid);

}

int main(){
    test(6);
    test(11);
    test(21);
    test(41);
    test(81);
    return 0;
}