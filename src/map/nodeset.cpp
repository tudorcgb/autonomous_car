#ifndef NODESET_H
#include "nodeset.h"
#endif

#ifndef HEURISTICS_H
#include "heuristics.h"
#endif // HEURISTICS_H

#include <climits>

int NodeSet::size()
{
    return this->size_;
}

Node NodeSet::operator[] (const std::string nodeName)
{
    if (this->nodesSet.find(nodeName) == this->nodesSet.end())
    {
        char message[40] ;
        strcpy(message, ("there is no Node with this name: " + nodeName).c_str());
        throw std::system_error(-1, std::generic_category(), message);
    }
    return *this->nodesSet[nodeName];
}

NodesVect NodeSet::operator[] (const std::complex<double> coord)
{
    NodesVect nodes;
    for (auto &nodeName : this->coordMap[coord])
    {
        nodes.push_back(this->nodesSet[nodeName]);
    }
    if (nodes.size() > 0)   { return nodes; }
    else
    {
        throw std::system_error(-1, std::generic_category(), "there is no Node at the specified coordinate");
    }
}

NODE_PTR NodeSet::nodePointer(const std::string nodeName)
{
    if (this->nodesSet.find(nodeName) == this->nodesSet.end())
    {
        char message[40] ;
        strcpy(message, ("there is no Node with this name: " + nodeName).c_str());
        throw std::system_error(-1, std::generic_category(), message);
    }
    return this->nodesSet[nodeName];
}

NODE_PTR NodeSet::closest(std::complex<double> coord)
{
    double min = MAXFLOAT;
    NODE_PTR n = NULL;
    for (auto &coordinates : this->coordMap)
    {
        double distance = euclideanHeuristic(coord, coordinates.first);
        if (distance < min)
        {
            min = distance;
            n = (*this)[coordinates.first][0];
        }
    }
    return n;
}