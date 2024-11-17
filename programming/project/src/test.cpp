#include "BSpline.h"
#include <iostream>
#include <vector>

int main() {
    // 测试数据：选择一个简单的节点序列
    vector<double> knots = {0.0, 1.0, 2.0, 3.0, 4.0}; 

    // 创建一个二阶（degree = 2）B样条对象
    B_base<3> b_spline(knots);

    // 调用 getBase 计算基函数
    b_spline.getBase();

    // 打印B样条的信息
    b_spline.print();



    return 0;
}
