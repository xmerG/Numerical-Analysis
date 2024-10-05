#include "Function.hpp"
#include "EquationSolver.hpp"
#include <iostream>
#include <cmath>

using namespace std;

class F1 : public Function {
public:
    double operator() (double x) const {
        return x-tan(x);
    }
};

void solve_f1() {
    cout << "Solving x - tan x near 4.5" << endl;
    Newton_Method solver_f1(F1(), 4.5);
    double x = solver_f1.solve();
    cout << "A root is: " << x << endl;

    cout << "Solving x - tan x near 7.7" << endl;
    Newton_Method solver_f2(F1(), 7.7);
    double y = solver_f2.solve();
    cout << "A root is: " << y<< endl;
}


int main() {
    solve_f1();
    return 0;
}
