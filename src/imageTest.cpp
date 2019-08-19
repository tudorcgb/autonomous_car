#if 0

#include "image.h"

int main(int argc, char **argv)
{
    Image img("/home/mihai/Workspace/BOSCH_2019/Dataset/16_apr/output_1555404694.9536414.avi");
    // Image img;
    for (int i = 0; i < 100; i++)
    {
        img.nextFrame();
        cv::imshow("image", img.frame());
        cv::waitKey();
    }
    img.close();

    return 0;
}

#endif