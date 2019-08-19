#ifndef PATH_H
#define PATH_H

#ifndef NODESET_H
#include "nodeset.h"
#endif // NODESET_H

#ifndef EDGESET_H
#include "edgeset.h"
#endif // EDGESET_H

class Path
{
public:
    /*!
     * \brief Construct a new Path object
     */
    Path();

    /*!
     * \name Path
     * \brief Construct a new Path object.
     * \param startingNode
     */
    Path(NODE_PTR startingNode, NODE_PTR destinationNode);

    /*!
     * \name Path
     * \brief Construct a new Path object
     * \param startingNode
     * \param destinationNode
     */
    Path(const std::string startingNode, const std::string destinationNode);

    /*!
     * \name Path
     * \brief Construct a new Path object
     * \param startingPosition
     * \param destinationPosition 
     */
    Path(const std::complex<double> startingPosition, const std::complex<double> destinationPosition);

    /*!
     * \name addNode
     * \brief Add a node to the path.
     * \param node      node to be added to path
     */
    void addNode(NODE_PTR node);

    /*!
     * \name removeNode
     * \brief Remove a node from the path.
     * \param node  Pointer to node that will be removed
     */
    void removeNode(NODE_PTR node);


    /*!
     * \name removeNode
     * \brief Remove a node from the path.
     * \param nodeName  name of the node to be removed
     */
    void removeNode(std::string nodeName);

    /*!
     * \name removeNodes
     * \brief Remove all nodes from a coordinates. 
     * \param coord     nodes coordinates
     */
    void removeNodes(std::complex<double> coord);

    /*!
     * \name aStar
     * \brief Searching algorithm to find the shorttest path from start to destination
     * \param startNode     starting node
     */
    void aStar(NODE_PTR startNode);
    
    /*!
     * \name startNode
     * \brief Getter for start_ node
     * \return NODE_PTR 
     */
    NODE_PTR startNode();

    /*!
     * \name destination
     * \brief Getter for destination_ node
     * \return NODE_PTR 
     */
    NODE_PTR destination();


    /*!
     * \name isDestination
     * \brief Verify if the current node is a destiantion node.
     * \param node  current node
     * \return 1 if the node is destination and 0 otherwise
     */
    bool isDestination(NODE_PTR node);

    /*!
     * \name makeEdges
     * \brief Create edges for all nodes of the path.
     */
    void makeEdges();

    /*!
     * \name closest
     * \brief Return the closest node to the specified coordinates
     * \param coord
     * \return NODE_PTR 
     */
    NODE_PTR closest(std::complex<double> coord);
    
    /*!
     * \name currentEdge
     * \brief Returns the edge coresponding edge for given coordinates
     * \param coord 
     * \return EDGE_PTR 
     */
    EDGE_PTR currentEdge(std::complex<double> coord);

    /*!
     * \name displacement
     * \brief Returns displacement from the path
     * \param coord     position
     * \return double 
     */
    double displacement(std::complex<double> coord);

    /*!
     * \name pathPos
     * \brief Returns position on the coresponding path element
     * \param coord 
     * \return std::complex<double> 
     */
    std::complex<double> pathPos(std::complex<double> coord);

    /*!
     * \name getNodesInPath
     * \brief Get the Nodes In Path object
     * \return const NodesVect 
     */
    const NodesVect getNodesInPath() const;

    /*!
     * \name getEdgesInPath
     * \brief Get the Edges In Path object
     * \return const EdgesVect 
     */
    const EdgesVect getEdgesInPath() const;

private:
    NODE_PTR start_;
    NODE_PTR destination_;

    NodesVect pathSet;
    EdgesVect edgePath;
    // NodesVect pathSet;
};

#endif // PATH_H