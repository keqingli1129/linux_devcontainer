#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "Lib/basics/lib.h"
#include "Lib/memory/shared.h"
#include "Lib/memory/unique.h"

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

    shared_demo::f1();
    unique_demo::f1();
    return 0;
}
