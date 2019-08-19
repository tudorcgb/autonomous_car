#if 0

#ifndef LINE_H
#include "line.h"
#endif // LINE_H

int main(int argc, char** argv)
{
    Line l0 = Line(0,0, 50,50);
    Line l1 = Line(80.50, 20.0, 20.0, 120.0);
    Line l2 = Line(std::complex<double>(2,2), std::complex<double>(3,3));
    // TODO: test with node and edge
    std::vector<cv::Point2f> line;
    l0.getLine(line);
    std::cout << "l0: "
              << line[0]
              << " "
              << line[1]
              << " "
              << l0.getColor()
              << " "
              << l0.getThickness()
              << std::endl;
    l0.setThickness(2);

    l1.getLine(line);
    std::cout << "l1: "
              << line[0]
              << " "
              << line[1]
              << " "
              << l1.getColor()
              << " "
              << l1.getThickness()
              << std::endl;
    l1.setThickness(2);

    l2.getLine(line);
    std::cout << "l2: "
              << line[0]
              << " "
              << line[1]
              << " "
              << l2.getColor()
              << " "
              << l2.getThickness()
              << std::endl;
    l2.setThickness(2);

    cv::Mat image = cv::imread("resources/Race_resize.png", CV_LOAD_IMAGE_COLOR);
    l0.draw(image);
    l1.draw(image);
    l2.draw(image);
    cv::imshow("image", image);
    cv::waitKey();

    return 0;
}

#endif