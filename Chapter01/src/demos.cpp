#include "Chapter01/demos.h"

#include "memory/shared.h"
#include "memory/unique.h"

namespace chapter01_demos
{
    void run_shared_ptr_demo()
    {
        shared_demo::f1();
    }

    void run_unique_ptr_demo()
    {
        unique_demo::f1();
    }
}
