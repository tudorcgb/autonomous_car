#ifndef MAPREPRESENTATION_H
#define MAPREPRESENTATION_H

#ifndef IMAGEPRIMITIVE_H
#include "imageprimitive.h"
#endif // IMAGEPRIMITIVE_H

#ifndef IMAGECOORD_H
#include "imagecoord.h"
#endif // IMAGECOORD_H

#ifndef EDGE_H
#include "edge.h"
#endif // EDGE_H

#ifndef LINE_H
#include "line.h"
#endif // LINE_H

#ifndef PATH_H
#include "path.h"
#endif // PATH_H

struct MouseParameters
{
    Path* p;
    cv::Mat image;
    ImageCoord* imgCoord;
};

class MapRepresentation
{
public:
    MapRepresentation(const std::string imagePath, const int startX=0, const int startY=0, const int endX=0, const int endY=0, const int distance=1);
    void addPrimitives(const std::vector<EDGE_PTR> edges);
    void addPrimitives(const std::vector<NODE_PTR> nodes);
    void addPrimitive(const NODE_PTR node);
    void addPrimitive(const EDGE_PTR edge);
    void setMouseCallbackParam(Path* pathObj);
    void drawAll();
    void showMap();
    cv::Point2f getPosition();
    static void readPosition(int  event, int  x, int  y, int  flag, MouseParameters *param);
private:
    std::string imagePath;
    cv::Mat image;
    // std::vector<IImagePrimitive> primitives;
    std::vector<Line*> primitives;
    ImageCoord imgCoord;
    cv::Point2f position;
    cv::Point2f pathPosition;
};

#endif // MAPREPRESENTATION_H