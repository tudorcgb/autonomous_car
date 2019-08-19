#if 0

#include "TrafficSignRecognition.h"
#include "image.h"
#include "timer.h"

int main(int argc, char **argv)
{
    // Image img ("/media/pi/PiDrive1/Workspace/TraffisSign/imgtovid.avi");
    Image img ("/media/pi/PiDrive1/Workspace/big_d/output_1555424209.8356912.avi");

    TrafficSignRecognition sign;
    Timer t("t", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();

    bool stop = false;
    bool parking = false;

    Timer t1("sign", 500000, std::bind(&TrafficSignRecognition::trafficSignInImageA, &sign, (void*)(NULL)));

    img.attachObserver(&sign);
    sleep(60);
    t.stop();
    img.close();
}

#endif