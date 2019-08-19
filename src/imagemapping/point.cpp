#ifndef POINT_H
#include "point.h"
#endif

Point::Point(const int x, const int y)
{
    this->point = cv::Point2f(x, y);
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Point::Point(const double x, const double y)
{
    this->point = cv::Point2f(x, y);
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Point::Point(std::complex<double> point)
{
    this->point = cv::Point2f(point.real(), point.imag());
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Point::Point(const Node& node)
{
    this->point = cv::Point2f(node.coord().real(), node.coord().imag());
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Point::Point(const Point& point)
{
    this->point = point.point;
    this->color = point.color;
    this->thickness = point.thickness;
}

Point& Point::operator=(const Point& point)
{
    this->point = point.point;
    this->color = point.color;
    this->thickness = point.thickness;
    return *this;
}

void Point::draw(cv::Mat& image) 
{
    cv::circle(image, this->point, thickness, this->color, this->thickness);
}

void Point::setColor(const cv::Vec3b& color)    { this-> color = color; }
void Point::setColor(const int red, 
                     const int green, 
                     const int blue)            { this->color = cv::Vec3b(blue, green, red); }
cv::Vec3b Point::getColor()                     { return this->color; }
void Point::setThickness(const int thickness)   { this->thickness = thickness; }
int Point::getThickness()                       { return this->thickness; }
cv::Point2f Point::getPoint()                   { return this->point; }
void Point::setPoint(cv::Point2f point)         { this->point = point; }
void Point::setPoint(const int x, const int y)  { this->point = cv::Point2f(x, y); }
void Point::setPoint(const double x, 
                     const double y)            { this->point = cv::Point2f(x,y); }
void Point::setPoint(const Node& node)          { this->point = cv::Point2f(node.coord().real(), node.coord().imag()); }