#if 0

#include "gpsconnection.h"
#include "pathtracking.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    GPSConnection gps(4, 12346);
    PathTracking path("NOD1", "NOD133");
    gps.attachObserver(&path);
    gps.start();
    sleep(60);
    gps.stop();
    std::cout << "complete test" << std::endl;
}

#endif