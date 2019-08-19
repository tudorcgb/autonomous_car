#ifndef NODE_H
#include "node.h"
#endif

Node::Node(Json::Value node)
{
    this->name_ = node["NAME"].asString();
    this->coord_ = std::complex<double>(
        node["COORDINATES"][0].asDouble(),
        node["COORDINATES"][1].asDouble());
    this->cost_ = 1;
    this->arcs_.push_back(node["IN_BACK"].asString());
    this->arcs_.push_back(node["IN_RIGHT"].asString());
    this->arcs_.push_back(node["IN_AHEAD"].asString());
    this->arcs_.push_back(node["IN_LEFT"].asString());

    this->arcs_.push_back(node["OUT_BACK"].asString());
    this->arcs_.push_back(node["OUT_RIGHT"].asString());
    this->arcs_.push_back(node["OUT_AHEAD"].asString());
    this->arcs_.push_back(node["OUT_LEFT"].asString());

    this->inBack = NULL; this->inRight = NULL; this->inAhead = NULL; this->inLeft = NULL;
    this->outBack = NULL; this->outRight = NULL; this->outAhead = NULL; this->outLeft = NULL;
    this->parent = NULL;

    this->linked_ = false;
}

Node::Node(Node& obj)
{
    this->name_      = obj.name();
    this->coord_     = obj.coord();
    this->cost_      = obj.cost();
    this->arcs_      = obj.arcs();
    this->inBack    = obj.inBack;
    this->inRight   = obj.inBack;
    this->inAhead   = obj.inAhead;
    this->inLeft    = obj.inLeft;
    this->outBack   = obj.outBack;
    this->outRight  = obj.outRight;
    this->outAhead  = obj.outAhead;
    this->outLeft   = obj.outLeft;
    this->linked_   = obj.linked();
    this->parent    = obj.parent;
}

Node& Node::operator=(Node& obj)
{
    this->name_      = obj.name();
    this->coord_     = obj.coord();
    this->cost_      = obj.cost();
    this->arcs_      = obj.arcs();
    this->inBack    = obj.inBack;
    this->inRight   = obj.inRight;
    this->inAhead   = obj.inAhead;
    this->inLeft    = obj.inLeft;
    this->outBack   = obj.outBack;
    this->outRight  = obj.outRight;
    this->outAhead  = obj.outAhead;
    this->outLeft   = obj.outLeft;
    this->linked_   = obj.linked();
    this->parent    = obj.parent;
    return *this;
}

std::ostream& operator<< (std::ostream& stream, const Node& node)
{
    if (&node == NULL)
    {
        return stream << "Node does not existt." << std::endl;
    }
    return stream   << "{ " 
                    << node.name()
                    << ",\t coord: "
                    << node.coord()
                    << " }";
}

// void Node::link(Dictionary *dict)
// {
//     this->inBack = dict->at(this->arcs[IN_BACK]);
//     this->inRight = dict->at(this->arcs[IN_RIGHT]);
//     this->inAhead = dict->at(this->arcs[IN_AHEAD]);
//     this->inLeft = dict->at(this->arcs[IN_LEFT]);
// }

int Node::successors(NodesVect& succ)
{
    if (this->linked())
    {
        if (this->outAhead != NULL)
        {
            succ.push_back(this->outAhead);
        }
        if (this->outBack != NULL)
        {
            succ.push_back(this->outBack);
        }
        if (this->outRight != NULL)
        {
            succ.push_back(this->outRight);
        }
        if (this->outLeft != NULL)
        {
            succ.push_back(this->outLeft);
        }
        return 0;
    }
    return -1;
}

// EDGE_PTR Node::operator[] (const std::string direction)
// {
//     return this->edges[direction];
// }

void Node::setLinked()                              { this->linked_ = true; }

const std::string& Node::name() const               { return this->name_; }
const std::complex<double>& Node::coord() const     { return this->coord_; }
const std::vector<std::string>& Node::arcs() const  { return this->arcs_; }
int Node::cost() const                              { return this->cost_; }
bool Node::linked() const                           { return this->linked_; }