#include <iostream>
#include <memory>
#include "unique.h"

namespace unique_demo
{
    void f1()
    {
        auto t = std::make_unique<ScopeTest>(10);
        t->test();
    }

    void f2()
    {
        auto *t2 = new ScopeTest(10);
        t2->test();
        delete t2;
    }
}
