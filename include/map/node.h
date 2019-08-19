#ifndef NODE_H
#define NODE_H

class Node;

#define NODE_PTR std::shared_ptr<Node>

// #ifndef DICTIONARY_H
// #include "dictionary.h"
// #endif

// #ifndef EDGE_H
// #include "edge.h"
// #endif

#include <iostream>
#include <string>
#include <json/json.h>
#include <complex>

typedef std::vector<NODE_PTR> NodesVect;
// typedef std::map<std::string, EDGE_PTR> Edges;

typedef enum arc
{
    IN_BACK = 0,
    IN_RIGHT,
    IN_AHEAD,
    IN_LEFT,
    OUT_BACK,
    OUT_RIGHT,
    OUT_AHEAD,
    OUT_LEFT
} Arcs;

// std::vector<std::string> nodeArcs = 
// {
//     "IN_BACK",
//     "IN_RIGHT",
//     "IN_AHEAD",
//     "IN_LEFT",
//     "OUT_BACK",
//     "OUT_RIGHT",
//     "OUT_AHEAD",
//     "OUT_LEFT"
// };

class Node
{
    // typedef std::map<std::string, Node*> Dictionary;
public:

    /*!
     * \name Node
     * \brief Construct for null node.
     */
    Node();

    /*!
     * \name Node
     * \brief Constructor method. 
     * \param node:      node extracted from json file
     */
    Node(Json::Value node);

    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param obj   Node object to be copied
     * \return copy of obj 
     */
    Node& operator=(Node& obj);

    /*!
     * \name Nodee
     * \brief Copy construcor.
     */
    Node(Node&);
    
    /*!
     * \name name
     * \brief Getter for name_.
     * \return name 
     */
    const std::string& name() const;
    
    /*!
     * \name coordinates
     * \brief Getter for coordinates_.
     * \return Coordinates 
     */
    const std::complex<double>& coord() const;

    /*!
     * \name cost
     * \brief Getter for cost_.
     * \return cost
     */
    int cost() const;

    /*!
     * \name arcs
     * \brief Getter for arcs_.
     * \return arcs
     */
    const std::vector<std::string>& arcs() const;

    /*!
     * \name linked
     * \brief Getter for linked_.
     * \return linked_
     */
    bool linked() const;

    // /*!
    //  * \name link
    //  * \brief This mothod makes connections to neighboring nodes.
    //  * \param dict  Dictionary that contains more nodes
    //  */
    // void link(Dictionary *dict);

    /*!
     * \name operator<<
     * \brief OStream redirect operator.
     * \param stream    String on the left
     * \param node      Data to be converted and added to string 
     * \return a concatenated ostream
     */
    friend std::ostream& operator<< (std::ostream& stream, const Node& node);

    /*!
     * \name linked
     * \brief Set the Linked attribute.
     */
    void setLinked();

    /*!
     * \name successors
     * \brief Put all the successors in succ vector only if the current node was linked
     * \param succ      list of successors
     * \return 0 if node is linked and -1 otherwise
     */
    int successors(NodesVect& succ);

    /*!
     * \name operator[]
     * \brief Returns pointer to the edge form the specified direction
     * \param direction
     * \return EDGE_PTR 
     */
    // EDGE_PTR operator[] (const std::string direction);

    NODE_PTR inBack;
    NODE_PTR inRight;
    NODE_PTR inAhead;
    NODE_PTR inLeft;
    NODE_PTR outBack;
    NODE_PTR outRight;
    NODE_PTR outAhead;
    NODE_PTR outLeft;

    NODE_PTR parent;

    // Edges edges;

    int cost_;
private:
    std::string name_;
    std::complex<double> coord_;
    std::vector<std::string> arcs_;
    bool linked_;
};

#endif // NODE_H