#ifndef MAPREPRESENTATION_H
#include "maprepresentation.h"
#endif // MAPREPRESENTATION_H

// #ifndef LINE_H
// #include "line.h"
// #endif // LINE_H

#ifndef POINT_H
#include "point.h"
#endif // POINT_H

MapRepresentation::MapRepresentation(const std::string imagePath, const int startX=0, const int startY=0, const int endX=0, const int endY=0, const int distance=1)
{
    this->imgCoord = ImageCoord(startX, startY, endX, endY, distance);
    this->imagePath = imagePath;
    this->image = cv::imread(this->imagePath, CV_LOAD_IMAGE_COLOR);
    this->pathPosition = cv::Point2f(0,0);
    cv::namedWindow("image");
    // cv::setMouseCallback("image", readPosition, (void*)(&this->position));
}

void MapRepresentation::setMouseCallbackParam(Path* pathObj)
{
    MouseParameters *m = new MouseParameters;
    m->p = pathObj;
    m->image = this->image;
    m->imgCoord = &this->imgCoord;
    cv::setMouseCallback("image", readPosition, m);
}

void MapRepresentation::addPrimitives(const std::vector<EDGE_PTR> edges)
{
    for (auto &edge : edges)
    {
        Line* line = new Line(*edge);
        line->setLine(this->imgCoord.toImageCoord(line->getP0()),
                     this->imgCoord.toImageCoord(line->getP1()));
        this->primitives.push_back(line);
    }
}
void MapRepresentation::addPrimitives(const std::vector<NODE_PTR> nodes) {}
void MapRepresentation::addPrimitive(const NODE_PTR node) {}

void MapRepresentation::drawAll()
{
    std::cout << "draw: " << this->primitives.size() << " primitives" << std::endl;
    for (auto &prim : this->primitives)
    {
        prim->draw(this->image);
    }
}

void MapRepresentation::showMap()
{
    if (image.rows > 0)
    {
        cv::imshow("image", this->image);
        cv::waitKey();
    }
    else
    {
        throw std::system_error(-1, std::generic_category(), "Image not open correctly");
    }
    
}

void MapRepresentation::readPosition(int  event, int  x, int  y, int  flag, MouseParameters *param)
{
    if (event == cv::EVENT_MOUSEMOVE)
    {
        // std::cout << "(" << x << ", " << y << ")" << std::endl;
        // param = (void*) new cv::Point2f(x, y);
        // param->displacement(std::complex<double>(x,y)) << std::endl;
        double px = (x - 23)/100.0;
        double py = (518 - (y - 23+22))/100.0;
        // std::cout << "x = " << px << " y = " << py << std::endl;
        std::cout << param->p->displacement(std::complex<double>(px,py)) << std::endl;
        std::complex<double> onPath = param->p->pathPos(std::complex<double>(px,py));
        // std::cout << onPath << std::endl;
        // cv::Point2f point = param->imgCoord->toImageCoord(cv::Point2f(onPath.real(), onPath.imag()));
        cv::Point2f point((onPath.real())*100 + 23, -((onPath.imag())*100)+518);
        // std::cout << point << std::endl;
        cv::Mat cop = param->image.clone();
        cv::circle(cop, cv::Point(x, y), 3, cv::Vec3b(0, 255, 255), 3);
        cv::line(cop, point, cv::Point(x, y), cv::Vec3b(0, 255, 255), 2);
        cv::circle(cop, point, 3, cv::Vec3b(200, 255, 50), 3);
        cv::imshow("image", cop);
    }
}

cv::Point2f MapRepresentation::getPosition()
{
    return this->position;
}