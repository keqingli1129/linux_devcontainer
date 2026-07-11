#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <open3d/Open3D.h>
#include "Chapter01/basics/lib.h"
#include "Chapter01/demos.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::int32_t a = 10;
    printer(a);

    auto name = std::string{"Jan"};
    std::cout << name << " has size " << name.size() << std::endl;
    name.append(" Schaffranek");
    std::cout << name << " has size " << name.size() << std::endl;

    // int y{7.7}; // illegal narrowing conversion from 'double' to 'int' [-Wnarrowing]
    std::vector<int> vec1{1, 2, 3, 4, 5};
    for (auto val = vec1.begin(); val != vec1.end(); val++) {
        std::cout << *val << " ";
    }
    std::cout << std::endl;

    chapter01_demos::run_shared_ptr_demo();
    chapter01_demos::run_unique_ptr_demo();
    // int my_variable{0};
    // {
    //     float my_variable{0.0};
    // }
    std::array<float, 2> f_arr{1.0, 2.0};
    std::cout << "Last element: " << f_arr.back() << std::endl;
    open3d::geometry::PointCloud pc;
    return 0;
}
