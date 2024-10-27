#include<iostream>
#include<cmath>
#include<vector>
#include<fstream>
#include"Polynomial.hpp"
#include"Function.hpp"
using namespace std;

const double pi = acos(-1.);
// F1 类继承自 Function 类
class F1 : public Function {
public:
    double operator() (double t) const {
        return sqrt(3) * cos(t);
    }
    double derivative(double t) const {
        return -sqrt(3)*sin(t);
    }
};

class F2:public Function{
    public:
        double operator() (double t) const {
            return (sqrt(3)*sin(t)+sqrt(abs(sqrt(3)*cos(t))))* 2.0 / 3.0;
        }
        double derivative(double t) const {
            double h=1e-6;
            return ((*this)(t+h)-(*this)(t))/h;
    }
};
class Curve{
    protected:
        const Function &F;
    private:
        vector<double> x={0.0};  //记录取的点pj的坐标
    public:
        Curve(const Function &F, vector<double>x):F(F),x(x){}
        vector<double> Cubic_Bezier(const vector<double> &_v){ 
            //输入由两个点的横坐标或纵坐标组成的vector
            vector<double> v(4); //拟合x(t),y(t)
            v[0]=F(_v[0]);
            v[1]=3*F(_v[0])+F.derivative(_v[0]);
            v[2]=3*F(_v[1])-F.derivative(_v[1]);
            v[3]=F(_v[1]);
            return v;
        }
        Polynomial get_piecewise_curve(){
            Polynomial p;
            vector<double> v=Cubic_Bezier(x);            
            p=Polynomial(vector<double>{v[0],-3*v[0],3*v[0], -v[0]})+
                Polynomial(vector<double>{0.0, v[1], -2*v[1], v[1]})+
                Polynomial(vector<double>{0.0, 0.0, v[2], -v[2]})+
                Polynomial(vector<double>{0.0, 0.0, 0.0, v[3]});
            return p;
        }
};
vector<double> Get_points(int n){
    double step =2*pi/n;
    vector<double>v(n+1);
    for(int i=0; i<=n; ++i){
        v[i]=i*step;
    }
    return v;
};
void save(int n){
    F1 f1;
    F2 f2;
     // 生成文件名
    std::string fileName = "F_for_n=" + to_string(n) + ".txt";

    // 打开文件输出流
    ofstream outfile(fileName);
    vector<double> t3=Get_points(n);
    for(int i =0; i<n; ++i){
        vector<double> v3{t3[i],t3[i+1]};
        Curve c3x(f1,v3);
        Curve c3y(f2,v3);
        Polynomial px3=c3x.get_piecewise_curve();
        Polynomial py3=c3y.get_piecewise_curve();
        px3.printforpy(outfile);
        py3.printforpy(outfile);
    }
    outfile.close();
};


int main(){
    save(10);
    save(40);
    save(160);
    return 0;
}






