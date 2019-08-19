#ifndef POINT_H
#define POINT_H

#ifndef IMAGEPRIMITIVE_H
#include "imageprimitive.h"
#endif // IMAGEPRIMITIVE_H

#ifndef NODE_H
#include "node.h"
#endif // NODE_H

class Point : public IImagePrimitive
{
public:
    Point(const int x, const int y);
    Point(const double x, const double y);
    Point(const std::complex<double> point);
    Point(const Node& node);
    Point(const Point& point);
    Point& operator=(const Point& point);
    // ~Point();
    void draw(cv::Mat& image);
    void setColor(const cv::Vec3b& color);
    void setColor(const int red, const int green, const int blue);
    cv::Vec3b getColor();
    void setThickness(const int thickness);
    int getThickness();
    cv::Point2f getPoint();
    void setPoint(cv::Point2f point);
    void setPoint(const int x, const int y);
    void setPoint(const double x, const double y);
    void setPoint(const Node& node);
private:
    cv::Point2f point;
};

#endif // POINT_H