#ifndef EDGE_H
#include "edge.h"

Edge::Edge(NODE_PTR from, NODE_PTR to, int cost_) : Vec2(from->coord(), to->coord())
{
    this->from_ = from;
    this->to_ = to;
    this->cost_ = cost_;
    double real = this->to_->coord().real() - this->from_->coord().real();
    double imag = this->to_->coord().imag() - this->from_->coord().imag();
    // std::cout << "In constructor is ok: " << this->orientation_ << std::endl;
    
}

Edge::Edge(Edge& edge) : Vec2((Vec2)edge)
{
    this->from_ = edge.from();
    this->to_ = edge.to();
    this->cost_ = edge.cost();
}

Edge& Edge::operator=(Edge& edge)
{
    this->from_ = edge.from();
    this->to_ = edge.to();
    this->cost_ = edge.cost();
    return *this;
}

const int Edge::cost() const                            { return this->cost_; }
void Edge::setCost(int cost_)                           { this->cost_ = cost_; }
// const std::complex<double>& Edge::orientation() const   { return this->orientation_; }
NODE_PTR Edge::from()                                   { return this->from_; }
NODE_PTR Edge::to()                                     { return this->to_; }


std::ostream& operator<< (std::ostream& stream, const Edge& edge)
{
    if (&edge == NULL)
    {
        return stream << "Edge does not exist." << std::endl;
    }
    return stream   << "{ " 
                    << edge.from()->name()
                    << " "
                    << edge.from()->coord()
                    << "\t-> "
                    << edge.to()->name()
                    << " "
                    << edge.to()->coord()
                    << ";\t orientation: "
                    << edge.orientation
                    << "}";
}

bool Edge::operator==(Edge& e)
{
    return (this->from()->name() == e.from()->name() && this->to()->name() == e.to()->name());
}

#endif // EDGE_H