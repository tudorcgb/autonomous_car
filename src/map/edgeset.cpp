#ifndef EDGESET_H
#include "edgeset.h"
#endif

int EdgeSet::edgeSize()     { return this->edgeSize_; }

Edge EdgeSet::edge(const std::string fromName, const std::string toName)
{
    return *this->edgesSet[std::make_tuple(fromName, toName)];
}

EDGE_PTR EdgeSet::pointerEdge(const std::string fromName, const std::string toName)
{
    if (this->edgesSet.find(std::make_tuple(fromName, toName)) == this->edgesSet.end())
    {
        return nullptr;
    }
    return this->edgesSet[std::make_tuple(fromName, toName)];
}

Edge EdgeSet::edge(const NODE_PTR from, NODE_PTR to)
{
    return *this->edgesSet[std::make_tuple(from->name(), to->name())];
}

// TODO: Not implemented
EdgesVect EdgeSet::edges(const std::complex<double> from, std::complex<double> to)
{

}

void EdgeSet::addEdge(EDGE_PTR edge)
{
    this->edgesSet[std::make_tuple(edge->from()->name(), edge->to()->name())] = edge;
    this->edgeSize_ = this->edgesSet.size();
}

void EdgeSet::addEdge(NODE_PTR from, NODE_PTR to, int cost_)
{
    if (from == NULL || to == NULL) return;
    EDGE_PTR edge = std::make_shared<Edge>(from, to, cost_);
    this->edgesSet[std::make_tuple(edge->from()->name(), edge->to()->name())] = edge;
    this->edgeSize_ = this->edgesSet.size();
}

void EdgeSet::removeEdge(const std::string fromName, const std::string toName)
{
    this->edgesSet.erase(std::make_tuple(fromName, toName));
    this->edgeSize_ = this->edgesSet.size();
}

bool EdgeSet::edgeExist(const std::string fromName, const std::string toName)
{
    if (this->edgesSet.count(std::make_tuple(fromName, toName)) > 0)
    {
        return true;
    }
    return false;
}