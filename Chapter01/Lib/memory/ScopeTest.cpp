#include "ScopeTest.h"

#include <iostream>

ScopeTest::ScopeTest(int val) : m_val(val)
{
    std::cout << "Constructor: " << m_val << '\n';
}

ScopeTest::~ScopeTest()
{
    std::cout << "Destructor!\n";
}

void ScopeTest::test()
{
    std::cout << m_val << '\n';
}
