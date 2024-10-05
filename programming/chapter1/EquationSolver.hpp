#ifndef EQUATIONSOLVER
#define EQUATIONSOLVER

#include "Function.hpp"
#include <cmath>

class EquationSolver{
protected:
    const Function & F;
public:
    EquationSolver(const Function& F) : F(F) {}
    virtual double solve() = 0;
};

class Bisection_Method : public EquationSolver {
private:
    double a, b;
    double eps, delta;
    int Maxiter;
public:
    Bisection_Method(const Function &F, double a, double b, 
        double eps = 1e-8, double delta = 1e-6, int Maxiter = 50) :
        EquationSolver(F), a(a), b(b), eps(eps), delta(delta), Maxiter(Maxiter) {}
    
    virtual double solve() {
        double h, u, c, w;
        h=b-a;
        u=F(a);
        for(int i=0; i<Maxiter+1;i++){
            h=h/2;
            c=a+h;
            if(h<delta || i==Maxiter){
                return a;
            }
            w=F(c);
            if(fabs(w)<eps){
                return c;
            }
            else if ((w<0&& u<0) || (w>0&&u>0))
                 a=c;      
        }
        }
};

class Newton_Method : public EquationSolver {
private:
    double x0;
    double eps;
    int Maxiter;
public:
    Newton_Method(const Function &F, double x0, 
        double eps = 1e-7, int  Maxiter = 10) :
        EquationSolver(F), x0(x0), Maxiter(Maxiter), eps(eps) {}
    
    virtual double solve() {
        double x, u, df;
        x=x0;
        for (int i=0; i<Maxiter+1; i++){
            u=F(x);
            if (fabs(u)<eps || i==Maxiter){
                return x;
            }
            else{
                df=F.derivative(x);
                x=x-F(x)/df;
            }            
        }
    }
};

class Secant_Method : public EquationSolver {
private:
    double x0, x1, eps, delta;
    int Maxiter;
public:
    Secant_Method(const Function &F,  double x0, double x1, 
        double delta=1e-6, double eps = 1e-8,  int  Maxiter = 100) :
        EquationSolver(F), x0(x0), x1(x1), delta(delta), Maxiter(Maxiter), eps(eps) {}
    
    virtual double solve() {
        double y0, y1, u, v, s;
        y0=x0;
        y1=x1;
        u=F(y1);
        v=F(y0);
       for (int i=2; i<Maxiter+1; i++){
            s=(y1-y0)/(u-v);
            y0=y1;
            v=u;
            y1=y1-u*s;
            if (fabs(u)<eps){
                return y1;
            }
            u=F(y1);
            if(fabs(y1-y0)<delta || i== Maxiter){
                return y1;
            }            
        }
    }
};


#endif