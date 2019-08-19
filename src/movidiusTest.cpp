#if 0

#include "movidiusnetwork.h"
#include "timer.h"
#include "image.h"

int main(int argc, char **argv)
{
    MovidiusNet net("/home/mihai/Workspace/BOSCH_2019/Dataset/video_process/snap_norm/graph");
    Image img("/home/mihai/Workspace/BOSCH_2019/Dataset/16_apr/output_1555405136.542821.avi");
    Timer t("t", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();
    img.attachObserver(&net);
    sleep(60);
    t.stop();
    img.close();

    // cv::Mat m = cv::imread("/home/mihai/Pictures/FoxSpeed.png");
    // cv::imshow("image", m);
    // cv::waitKey();

    return 0;
}

#endif