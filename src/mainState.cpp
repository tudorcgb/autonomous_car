#if 1

#include "statemachine.h"
#include <unistd.h>

Machine m;

void interruptHandler(sig_t s){
    m.gps.stop();
    m.carControl.brake(0.0);
    m.carControl.close();
    exit(1); 
}

int main(int argc, char **argv)
{
    signal (SIGINT, interruptHandler);
    m.waitForGPS = atoi(argv[1]);
    Timer t("machine", 50000, std::bind(&Machine::switchState, &m, (void*)(NULL)));
    t.start();

    // std::thread switchState(&Machine::switchState, &m, (void*)(NULL));

    m.run(NULL);

    // sleep(10000000);
}

#endif