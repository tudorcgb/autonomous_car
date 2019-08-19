#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <list>
#include <json/json.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <complex>
#include <fstream>


// #include "node.h"
#ifndef NODESET_H
#include "nodeset.h"
#endif // NODESET_H

#ifndef EDGESET_H
#include "edgeset.h"
#endif // EDGESET_H

// TODO: remove commented
// template<typename T> struct less {};
//     template <typename T>
//     struct less<std::complex<T> >
//     {
//         bool operator() (std::complex<T> const& a, std::complex<T> const&b)
//         {
//             return std::array<T,2>{a.real(), a.imag()} < std::array<T,2>{b.real(), b.imag()};
//         }
//     };

// typedef std::map<std::string, NODE_PTR> Dictionary;
// typedef std::map<std::complex<double>, std::vector<std::string>, struct less<std::complex<double> >> CoordMapping;

class Map : public NodeSet, public EdgeSet
{
public:

    /*!
     * \name getInstance
     * \brief Get the Instance object.
     * \return an instance of map
     */
    static Map& getInstance();

    // TODO: remove commented
    // /*!
    //  * \name size
    //  * \brief Returns number of nodes in map.
    //  * \return number of nodes in map
    //  */
    // int size();

    /*!
     * \name setGoal
     * \brief Set the Goal node.
     * \param nodeName  destination node
     */
    void setGoal(const std::string nodeName);

    /*!
     * \name setGoal
     * \brief Set the Goal node.
     * \param coordinates   represents coordinates of the destination node
     */
    void setGoal(const std::complex<double> coordinates);

    /*!
     * \name setStrat
     * \brief Set the Start node.
     * \param nodeName  start node
     */
    void setStart(const std::string nodeName);

    /*!
     * \name setStart
     * \brief Set the Start node.
     * \param coordinates   coordonates of the start node
     */
    void setStart(const std::complex<double> coordinates);

    /*!
     * \name linkNodes
     * \brief Construct a graph from all nodes.
     */
    void linkNodes();

    // // TODO: remove commented
    // /*!
    //  * \name operator[]
    //  * \brief Returns the node with specified name from the map.
    //  * \param nodeName  name of the desired node
    //  * \return Node 
    //  */
    // Node operator[] (const std::string nodeName);

    // /*!
    //  * \name operator[]
    //  * \brief Returns the nodes placed at the specidied coordinates.
    //  * \param coord     coordinates of the desired nodes
    //  * \return std::vector<Node> 
    //  */
    // NodesVect operator[] (const std::complex<double> coord);

    NODE_PTR goal;             // TODO: private
    NODE_PTR start;            // TODO: private
    // Dictionary nodesMap;    // TODO: private
    // int mapSize;            // TODO: private
    // CoordMapping coordMap;  // TODO: private

private:

    /*!
     * \name Map
     * \brief Construct a new Map object.
     * \param jsonMap   path to json file representing map
     */
    Map(const std::string jsonMap);

    /*!
     * \name Map
     * \brief Copy Constructor.
     * \param obj   object to pe copied
     */
    Map(const Map& obj);
    
    /*!
     * \name operator=
     * \brief Assignment operator.
     * \param obj   object to be copied
     * \return Map& 
     */
    Map& operator=(const Map& obj);
};

#endif // MAP_H