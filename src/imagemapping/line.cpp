#ifndef LINE_H
#include "line.h"
#endif // LINE_H

Line::Line(const int x0, const int y0, const int x1, int y1)
{
    this->p0 = cv::Point2f(x0, y0);
    this->p1 = cv::Point2f(x1, y1);
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Line::Line(const double x0, const double y0, const double x1, const double y1)
{
    this->p0 = cv::Point2f(x0, y0);
    this->p1 = cv::Point2f(x1, y1);
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Line::Line(const std::complex<double> p0, const std::complex<double> p1)
{
    this->p0 = cv::Point2f(p0.real(), p0.imag());
    this->p1 = cv::Point2f(p1.real(), p1.real());
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Line::Line(const Node& node0, const Node& node1)
{
    this->p0 = cv::Point2f(node0.coord().real(), node0.coord().imag());
    this->p1 = cv::Point2f(node1.coord().real(), node1.coord().imag());
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Line::Line(Edge& edge)
{
    this->p0 = cv::Point2f(edge.from()->coord().real(), edge.from()->coord().imag());
    this->p1 = cv::Point2f(edge.to()->coord().real(), edge.to()->coord().imag());
    this->color = cv::Vec3b(0, 0, 255);
    this->thickness = 1;
}

Line::Line(const Line& line)
{
    this->p0 = line.p0;
    this->p1 = line.p1;
    this->color = line.color;
    this->thickness = line.thickness;
}

Line& Line::operator=(const Line& line)
{
    this->p0 = line.p0;
    this->p1 = line.p1;
    this->color = line.color;
    this->thickness = line.thickness;
    return *this;
}

void Line::draw(cv::Mat& image) 
{
    cv::arrowedLine(image, this->p0, this->p1, this->color, this->thickness);
}

void Line::setColor(const cv::Vec3b& color)     { this->color = color; }
void Line::setColor(const int red, 
                    const int green, 
                    const int blue)             { this->color = cv::Vec3b(blue, green, red); }
cv::Vec3b Line::getColor()                      { return this->color; }
void Line::setThickness(const int thickness)    { this->thickness = thickness; }
int Line::getThickness()                        { return this->thickness; }
cv::Point2f Line::getP0()                       { return this->p0; }
cv::Point2f Line::getP1()                       { return this->p1; }
void Line::getLine(std::vector<cv::Point2f> &line) 
{
    line.clear();
    line.push_back(this->p0);
    line.push_back(this->p1);
}
void Line::setLine(const int x0, const int y0, const int x1, const int y1)
{
    this->p0 = cv::Point2f(x0, y0);
    this->p1 = cv::Point2f(x1, y1);
}
void Line::setLine(const double x0, const double y0, const double x1, const double y1)
{
    this->p0 = cv::Point2f(x0, y0);
    this->p1 = cv::Point2f(x1, y1);
}
void Line::setLine(const std::complex<double> p0, const std::complex<double> p1)
{
    this->p0 = cv::Point2f(p0.real(), p0.imag());
    this->p1 = cv::Point2f(p1.real(), p1.real());
}
void Line::setLine(const Node& node0, const Node& node1)
{
    this->p0 = cv::Point2f(node0.coord().real(), node0.coord().imag());
    this->p1 = cv::Point2f(node1.coord().real(), node1.coord().imag());
}
void Line::setLine(Edge& edge)
{
    this->p0 = cv::Point2f(edge.from()->coord().real(), edge.from()->coord().imag());
    this->p1 = cv::Point2f(edge.to()->coord().real(), edge.to()->coord().imag());
}
void Line::setLine(const cv::Point2f p0, const cv::Point2f p1)
{
    this->p0 = p0;
    this->p1 = p1;
}