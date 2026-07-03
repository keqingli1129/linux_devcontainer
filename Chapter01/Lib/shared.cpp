#include "shared.h"

#include <iostream>
#include <memory>

void f1()
{
    auto t = std::make_shared<ScopeTest>(10);
    t->test();

    std::cout << "Count: " << t.use_count() << '\n';

    {
        auto t2 = t;
        t2->test();

        std::cout << "Count: " << t.use_count() << '\n';
    }

    std::cout << "Count: " << t.use_count() << '\n';
}

void f2()
{
    auto t4 = std::make_shared<ScopeTest>(10);
    std::cout << "Count t4: " << t4.use_count() << '\n';
    auto t5 = std::make_shared<ScopeTest>(10);
    std::cout << "Count t5: " << t5.use_count() << '\n';

    t4->m_partner = t5;
    std::cout << "Count t5: " << t5.use_count() << '\n';
    t5->m_partner = t4;
    std::cout << "Count t4: " << t4.use_count() << '\n';
}
