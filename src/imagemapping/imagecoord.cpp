#ifndef IMAGECOORD_H
#include "imagecoord.h"
#endif // IMAGECOORD_H

ImageCoord::ImageCoord()
{
    this->startPoint = cv::Point2f(0, 0);
    this->endPoint = cv::Point2f(0, 0);
    this->pointDistance = 1;
}

ImageCoord::ImageCoord(const int startX, const int startY, const int endX, const int endY, const int distance)
{
    this->startPoint = cv::Point2f(startX, startY);
    this->endPoint = cv::Point2f(endX, endY);
    this->pointDistance = distance;
}

ImageCoord::ImageCoord(const ImageCoord& i)
{
    this->startPoint = i.startPoint;
    this->endPoint = i.endPoint;
    this->pointDistance = i.pointDistance;
}

ImageCoord& ImageCoord::operator=(const ImageCoord& i)
{
    this->startPoint = i.startPoint;
    this->endPoint = i.endPoint;
    this->pointDistance = i.pointDistance;
    return *this;
}

cv::Point2f ImageCoord::toImageCoord(const std::complex<double> point)
{
    double x = point.real()*100 + this->startPoint.x;
    double y = point.imag()*100 + this->startPoint.y;
    if ( (x < this->endPoint.x + this->pointDistance && y < this->endPoint.y + this->pointDistance) &&
        (x > this->startPoint.x + this->pointDistance && y > this->startPoint.y + this->pointDistance) )
    {
        return cv::Point2f((int)x, (int(y)));
    }
    throw std::system_error(-1, std::generic_category(), "Point outside the map");
}

cv::Point2f ImageCoord::toImageCoord(const double x, const double y)
{
    double tmpX = x*100 + this->startPoint.x;
    double tmpY = y*100 + this->startPoint.y;
    if ((tmpX <= this->endPoint.x + this->pointDistance && tmpY <= this->endPoint.y + this->pointDistance) &&
        (tmpX >= this->startPoint.x - this->pointDistance && tmpY >= this->startPoint.y - this->pointDistance) )
    {
        return cv::Point2f((int)tmpX, (int(tmpY)));
    }
    throw std::system_error(-1, std::generic_category(), "Point outside the map");
}

cv::Point2f ImageCoord::toImageCoord(const cv::Point2f point)
{
    double x = point.x*100 + this->startPoint.x;
    double y = -point.y*100+this->endPoint.y-22 + this->startPoint.y;
    std::cout << x << " " << y << std::endl;
    // if ( (x < this->endPoint.x + this->pointDistance && y < this->endPoint.y + this->pointDistance) &&
    //     (x > this->startPoint.x + this->pointDistance && y > this->startPoint.y + this->pointDistance) )
    {
        return cv::Point2f((int)x, (int(y)));
    }
    throw std::system_error(-1, std::generic_category(), "Point outside the map");
}

cv::Point2f ImageCoord::toMapCoord(const cv::Point2f point)
{
    double x = (point.x - this->startPoint.x)/100.0;
    double y = (point.x - this->startPoint.y+22)/100.0;
    return cv::Point2f(x, y);
}