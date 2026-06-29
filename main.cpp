#include<iostream>
#include "Chapter01/Lib/lib.h"
#include <Eigen/Dense>

using namespace std;
int main() {
    cout << "Hello, World!" << endl;
    int32_t a = 10;
    auto b = 20.0F;
    printer(a);
    const auto c = 10;
    constexpr auto e = 10;
    constexpr auto len = 5U;
    uint32_t myarray[len] = {0};
    // Eigen sample: 2x2 matrix multiplication
    Eigen::Matrix2d mat;
    mat << 1, 2,
           3, 4;
    Eigen::Vector2d vec(5, 6);
    Eigen::Vector2d result = mat * vec;
    cout << "Matrix:\n" << mat << endl;
    cout << "Vector: " << vec.transpose() << endl;
    cout << "Result: " << result.transpose() << endl;

    return 0;
}
