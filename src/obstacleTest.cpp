#if 1

#include "ObstacleDetection.h"
#include "image.h"
#include "timer.h"

int main()
{
    ObstacleDetection obstacleDetection;
    Image img("/media/pi/PiDrive1/Workspace/output_1555424602.779776.avi");
    Timer t("t", 500000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();
    img.attachObserver(&obstacleDetection);
    sleep(6000);
    t.stop();
    img.close();

    return 0;
}

#endif