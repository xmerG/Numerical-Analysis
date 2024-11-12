#ifndef FUNCTION
#define FUNCTION

class Function {
public:
    virtual double operator() (double x) const = 0;
    virtual double derivative(double x) const {
        double h=1e-6;
        return ((*this)(x+h)-(*this)(x))/h;
    }
    double doubleDerivative(double x) const{
        double h=1e-6;
        return ((*this).derivative(x+h)-(*this).derivative(x))/h;
    }
};

#endif