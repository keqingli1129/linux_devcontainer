#pragma once

#include <iostream>
#include <memory>

class ScopeTest
{
public:
    ScopeTest(int val);
    ~ScopeTest();
    void test();
    std::shared_ptr<ScopeTest> m_partner;

private:
    int m_val;
};

void f1();
void f2();
