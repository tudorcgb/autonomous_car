#ifndef EDGESET_H
#define EDGESET_H

#include <iostream>

#ifndef EDGE_H
#include "edge.h"
#endif // EDGE_H

typedef std::vector<EDGE_PTR> EdgesVect;
typedef std::map<std::tuple<std::string, std::string>, EDGE_PTR> EdgeDict;

class EdgeSet
{
public:
    /*!
    * \name size
    * \brief Returns number of edges in set.
    * \return number of nodes in set
    */
    virtual int edgeSize();

    /*!
     * \name edge
     * \brief Returns the edge formed by specified nodes.
     * \param fromName  From node
     * \param toName    To node
     * \return Edge 
     */
    virtual Edge edge(const std::string fromName, const std::string toName);

    /*!
     * \name edge
     * \brief Returns a pointer to the edge formed by specified nodes.
     * \param fromName  From node
     * \param toName    To node
     * \return Edge 
     */
    virtual EDGE_PTR pointerEdge(const std::string fromName, const std::string toName);

    /*!
     * \name edge
     * \brief Returns the edge formed by specified nodes.
     * \param from  From node
     * \param to    To node
     * \return Edge 
     */
    virtual Edge edge(const NODE_PTR from, NODE_PTR to);

    /*!
     * \name edges
     * \brief Returns all the edges with specified coordinates.
     * \param from      form coordinates
     * \param to        to coordinates
     * \return EdgesVect 
     */
    virtual EdgesVect edges(const std::complex<double> from, std::complex<double> to);

    /*!
     * \name addEdge
     * \brief Add the adge to the set.
     * \param edge 
     */
    virtual void addEdge(EDGE_PTR edge);
    
    /*!
     * \name addEdge
     * \brief Create and add the edge to the set.
     * \param form  from node
     * \param to    to node
     */
    virtual void addEdge(NODE_PTR form, NODE_PTR to, int cost_);

    /*!
     * \name removeEdge
     * \brief Remove edge from the set.
     * \param fromName  From node
     * \param toName    To node
     */
    virtual void removeEdge(const std::string fromName, const std::string toName);

    /*!
     * \name edgeExist
     * \brief Check if an edge exists in th edgesSet.
     * \param fromName 
     * \param toName 
     * \return true 
     * \return false 
     */
    virtual bool edgeExist(const std::string fromName, const std::string toName);

    EdgeDict edgesSet;

private:
    int edgeSize_;
};

#endif // EDGESET_H