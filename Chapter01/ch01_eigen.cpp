#include <iostream>

#include <Eigen/Dense>

int main() {
    // 2x2 matrix multiplication
    Eigen::Matrix2d mat;
    mat << 1, 2,
           3, 4;
    Eigen::Vector2d vec(5, 6);
    Eigen::Vector2d result = mat * vec;
    std::cout << "Matrix:\n" << mat << std::endl;
    std::cout << "Vector: " << vec.transpose() << std::endl;
    std::cout << "Result: " << result.transpose() << std::endl;
    return 0;
}
