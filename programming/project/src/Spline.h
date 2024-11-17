#ifndef _SPLINE_H_
#define _SPLINE_H_
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
using namespace std;
enum class SplineType{
    ppForm,
    BForm
};

template<int dim, int order, SplineType>
class Spline{

};

#endif