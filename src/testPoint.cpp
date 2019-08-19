#if 0

#ifndef POINT_H
#include "point.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/imgcodecs.hpp"
#endif // POINT_H

int main(int argc, char** argv)
{
    Point p0 = Point(20, 20);
    Point p1 = Point(30.0, 30.0);
    Point p2 = Point(std::complex<double>(40.0, 40.0));
    p0.setThickness(3);
    p1.setThickness(5);
    p2.setThickness(7);
    // TODO: test with node
    std::cout << "p0: " << p0.getPoint() << " " << p0.getColor() << " " << p0.getThickness() << std::endl;
    std::cout << "p1: " << p1.getPoint() << " " << p1.getColor() << " " << p1.getThickness() << std::endl;
    std::cout << "p2: " << p2.getPoint() << " " << p2.getColor() << " " << p2.getThickness() << std::endl;

    cv::Mat image = cv::imread("resources/Race_resize.png", CV_LOAD_IMAGE_COLOR);
    p0.draw(image);
    p1.draw(image);
    p2.draw(image);
    cv::imshow("image", image);
    cv::waitKey();

    return 0;
}

#endif