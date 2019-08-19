#if 0

#include <functional>

#include "statemachine.h"

int main()
{
    Machine m;
    Timer t1("m", 50000, std::bind(&Machine::run, &m, (void*)(NULL)));

    t1.start();
    sleep(10000);
    return 0;
}

#endif