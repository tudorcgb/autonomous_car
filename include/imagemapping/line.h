#ifndef LINE_H
#define LINE_H

#ifndef IMAGEPRIMITIVE_H
#include "imageprimitive.h"
#endif // IMAGEPRIMITIVE_H

#ifndef EDGE_H
#include "edge.h"
#endif // EDGE_H

class Line : public IImagePrimitive
{
public:
    Line(const int x0, const int y0, const int x1, const int y1);
    Line(const double x0, const double y0, const double x1, const double y1);
    Line(const std::complex<double> p0, const std::complex<double> p1);
    Line(const Node& node0, const Node& node1);
    Line(Edge& edge);
    Line(const Line& line);
    Line& operator=(const Line& line);
    void draw(cv::Mat& image);
    void setColor(const cv::Vec3b& color);
    void setColor(const int red, const int green, const int blue);
    cv::Vec3b getColor();
    void setThickness(const int thickness);
    int getThickness();
    void getLine(std::vector<cv::Point2f> &line);
    void setLine(const int x0, const int y0, const int x1, const int y1);
    void setLine(const double x0, const double y0, const double x1, const double y1);
    void setLine(const std::complex<double> p0, const std::complex<double> p1);
    void setLine(const Node& node0, const Node& node1);
    void setLine(Edge& edge);
    void setLine(const cv::Point2f p0, const cv::Point2f p1);
    cv::Point2f getP0();
    cv::Point2f getP1();
private:
    cv::Point2f p0;
    cv::Point2f p1;
};

#endif // LINE_H