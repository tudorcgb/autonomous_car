#ifndef PATHTRACKING_H
#include "pathtracking.h"
#endif // PATHTRACKING_H

#ifndef MAP_H
#include "map.h"
#endif // MAP_H

#include <time.h>
#include <thread>
#include <unistd.h>

#define CAR_ID 4

PathTracking::PathTracking(std::complex<double> startingNode, std::complex<double> destinationNode) :
    path(Path(Map::getInstance()[startingNode][0], Map::getInstance()[destinationNode][0])),
    running(true), crossIndex(0),
    displ_(0.0)  
    {
        this->globalPosition_ = GPSData(CAR_ID, this->path.getNodesInPath()[0]->coord(), std::complex<double>(0,0));
        this->pathPosition_.update(this->path.pathPos(this->globalPosition_.getPosition()), this->path.pathPos(this->globalPosition_.getOrientation()));
    }

PathTracking::PathTracking(const std::string startingNode, const std::string destinationNode) :
    path(Path(Map::getInstance().nodePointer(startingNode), Map::getInstance().nodePointer(destinationNode))),
    running(true), crossIndex(0),
    displ_(0.0)  
    {
        this->globalPosition_ = GPSData(CAR_ID, this->path.getNodesInPath()[0]->coord(), std::complex<double>(0,0));
        this->pathPosition_.update(this->path.pathPos(this->globalPosition_.getPosition()), this->path.pathPos(this->globalPosition_.getOrientation()));
    }

// PathTracking::PathTracking(const PathTracking& pathTracking)
// {
//     this->globalPosition_ = pathTracking.globalPosition_;
//     this->pathPosition_ = pathTracking.pathPosition_;
//     this->running = pathTracking.running;
//     this->displ_ = pathTracking.displ_;
//     this->path = pathTracking.path;
// }

void PathTracking::run()
{

}

std::complex<double> PathTracking::nextNode()
{
    return this->path.currentEdge(this->globalPosition_.getPosition())->to()->coord();
}

int PathTracking::nextNodes(const int nodesNo, std::vector<std::complex<double> >& nodes)
{
    EDGE_PTR e = path.currentEdge(this->globalPosition_.getPosition());
    NODE_PTR n = e->to();
    nodes.push_back(n->coord());
    int index;
    NodesVect vect = this->path.getNodesInPath();
    for (index = 0; index < vect.size(); index++)
    {
        if (vect[index]->coord() == n->coord())
        {
            break;
        }
    }
    for (int i = 1; i < nodesNo; i++)
    {
        if (index + i >= vect.size())
        {
            return i;
        }
        nodes.push_back(vect[index + i]->coord());
    }
    return nodesNo;
}

int PathTracking::nextObstaclesNodes(const std::complex<double> position, int nodesNo, std::vector<std::complex<double> >& nodes)
{
    EDGE_PTR e = path.currentEdge(position);
    NODE_PTR n = e->to();
    std::complex<double> v = e->normal();
    if (e->direction(e->to()->coord()+e->normal()) > 0)
    {
        nodes.push_back(e->to()->coord() + e->normal()*0.225);
    }
    else
    {
        nodes.push_back(e->to()->coord() + e->normal()*(-0.225));
    }

    int index;
    NodesVect vect = this->path.getNodesInPath();
    for (index = 0; index < vect.size(); index++)
    {
        if (vect[index]->coord() == n->coord())
        {
            break;
        }
    }
    for (int i = 1; i < nodesNo; i++)
    {
        if (index + i >= vect.size())
        {
            return i;
        }
        if (i >= nodesNo - 2)
        {
            nodes.push_back(vect[index + i]->coord());
        }
        else if (i == nodesNo - 3)
        {
            if (e->direction(e->to()->coord()+e->normal()) > 0)
            {
                nodes.push_back(vect[index + i]->coord() + e->normal()*0.112);
            }
            else
            {
                nodes.push_back(vect[index + i]->coord() + e->normal()*(-0.112));
            }
        }
        else
        {
            std::complex<double> node;
            if (e->direction(e->to()->coord()+e->normal()) > 0)
            {
                // nodes.push_back(vect[index + i]->coord() + e->normal()*0.225);
                node = ((vect[index + i]->coord() + e->normal()*0.225) + nodes[i-1]);
            }
            else
            {
                // nodes.push_back(vect[index + i]->coord() + e->normal()*(-0.225));
                node = ((vect[index + i]->coord() + e->normal()*(-0.225)) + nodes[i-1]);
            }
            nodes.push_back(std::complex<double>(node.real()/2.0, node.imag()/2.0));
        }
    }
    return nodesNo;
}

void PathTracking::addToCrossRoad(const std::complex<double> position)
{
    this->crossRoad.push_back(position);
}

double PathTracking::distanceToCross()
{
    double min = 600.0;
    for (int index = 0; index < this->crossRoad.size(); index++)
    // for (std::complex<double> point : this->crossRoad)
    {
        std::complex<double> point = this->crossRoad.at(index);
        double dist = sqrt(
            (
                point.real() - this->globalPosition_.getPosition().real()
            )*(
                point.real() - this->globalPosition_.getPosition().real()
            ) 
            + 
            (
                point.imag() - this->globalPosition_.getPosition().imag()
            )*(
                point.imag() - this->globalPosition_.getPosition().imag()
            )
        );
        if (dist < min)
        {
            min = dist;
            this->crossIndex = index;
        }
    }
    return min;
}

double PathTracking::distanceToGoal()
{
    std::complex<double> point = this->path.destination()->coord();
    double dist = sqrt(
        (
            point.real() - this->globalPosition_.getPosition().real()
        )*(
            point.real() - this->globalPosition_.getPosition().real()
        ) 
        + 
        (
            point.imag() - this->globalPosition_.getPosition().imag()
        )*(
            point.imag() - this->globalPosition_.getPosition().imag()
        )
    );
    return dist;
}

void PathTracking::update(Subject* /*gps*/ imuPos)
{
    // this->globalPosition_ = ((GPSConnection*)gps)->position;
    // this->pathPosition_.update(this->path.pathPos(this->globalPosition_.getPosition()), this->path.pathPos(this->globalPosition_.getOrientation()));
    // this->displ_ = this->path.displacement(this->globalPosition_.getPosition());
    // std::cout << this->pathPosition_ << " displ: " << this->displ_ << std::endl;
    float x = ((IMUEncoderPosition*)imuPos)->getX();
    float y = ((IMUEncoderPosition*)imuPos)->getY();
    this->globalPosition_ = GPSData(this->globalPosition_.getId(), std::complex<double>((x-27.0)/100.0, ((double)521.5-y)/100.0), this->globalPosition_.getOrientation());
    this->pathPosition_.update(this->path.pathPos(this->globalPosition_.getPosition()), this->path.pathPos(this->globalPosition_.getOrientation()));
    this->displ_ = this->path.displacement(this->globalPosition_.getPosition());
} 

void PathTracking::update(double x, double y)
{
    this->globalPosition_ = GPSData(0, std::complex<double>(x, y), std::complex<double>(0,0));
    this->pathPosition_.update(this->path.pathPos(this->globalPosition_.getPosition()), this->path.pathPos(this->globalPosition_.getOrientation()));
    this->displ_ = this->path.displacement(this->globalPosition_.getPosition());
}

GPSData PathTracking::globalPosition()      { return this->globalPosition_; }
GPSData PathTracking::pathPosition()        { return this->pathPosition_; }
double PathTracking::displacement()         { return this->displ_; }