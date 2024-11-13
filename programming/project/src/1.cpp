#include <iostream>
#include <vector>
#include <lapacke.h>
#include <iostream>
#include <vector>
#include <lapacke.h>

using namespace std;

int main() {
    // 定义矩阵 A 和向量 b
    int n = 3;  // A 是一个 3x3 的矩阵
    vector<double> A = { 3, 2, -1,
                         2, -1, 1,
                         1, 2, 3 };
    vector<double> b = { 1, 2, 3 };  // 右侧向量 b

    // 用来存储行交换信息的数组
    vector<int> ipiv(n);

    // 调用 LAPACKE_dgesv 函数来求解 Ax = b
    int info = LAPACKE_dgesv(LAPACK_COL_MAJOR, n, 1, A.data(), n, ipiv.data(), b.data(), n);

    // 检查是否成功
    if (info != 0) {
        cout << "求解失败！错误代码：" << info << endl;
    } else {
        cout << "解向量 x：" << endl;
        for (int i = 0; i < n; i++) {
            cout << b[i] << endl;  // 输出解向量 x
        }
    }

    return 0;
}
