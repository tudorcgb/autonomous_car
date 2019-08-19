#ifndef EDGE_H
#define EDGE_H

#ifndef NODE_H
#include "node.h"
#endif // NODE__H

#ifndef VEC2_H
#include "vec2.h"
#endif // VEC2_H

#define EDGE_PTR std::shared_ptr<Edge>

class Edge : public Vec2
{
public:
    /*!
     * \name Edge
     * \brief Construct a new Edge object.
     * \param from      first node of the Edge
     * \param to        second node of the Edge
     * \param cost_     cost of the edge
     */
    Edge(NODE_PTR from, NODE_PTR to, int cost_);

    /*!
     * \name Edge
     * \brief Copy Contructor.
     * \param edge  edge to be copied.
     */
    Edge(Edge& edge);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param edge  edge to be copied
     * \return Edge& 
     */
    Edge& operator=(Edge& edge);

    /*!
     * \name cost
     * \brief Getter for cost_.
     * \return cost_
     */
    const int cost() const;

    /*!
     * \name setCost
     * \brief Set the Cost object.
     * \param cost_     cost of the edge
     */
    void setCost(int cost_);
    
    // /*!
    //  * \name orientation
    //  * \brief Returns the orientation of the edge
    //  * \return std::complex<double> 
    //  */
    // const std::complex<double>& orientation() const;

    /*!
     * name from
     * \brief Getter for form_ node.
     * \return NODE_PTR 
     */
    NODE_PTR from();
    
    /*!
     * \name to
     * \brief Getter for to_ node.
     * \return NODE_PTR 
     */
    NODE_PTR to();

    /*!
     * \name operator<<
     * \brief OStream redirect operator.
     * \param stream    String on the left
     * \param edge      Data to be converted and added to string 
     * \return a concatenated ostream
     */
    friend std::ostream& operator<< (std::ostream& stream, const Edge& edge);

    /*!
     * \name operator==
     * \brief Compare 2 objects of type Edge
     * \param e 
     * \return true 
     * \return false 
     */
    bool operator==(Edge& e);

private:
    NODE_PTR from_;
    NODE_PTR to_;
    int cost_;
    std::complex<double> orientation_;
};

#endif // EDGE_H