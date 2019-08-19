#ifndef MAP_H
#include "map.h"
#endif

Map::Map(const std::string jsonFilePath)
{
    Json::Reader reader;
    Json::Value root;
    std::string line;
    std::string jsonMap;
    std::ifstream jsonFile(jsonFilePath);

    if (jsonFile.is_open())
    {
        while (getline(jsonFile, line))
        {
            jsonMap += line;
        }
    }
    else 
    {
        std::cerr << "Map: " << strerror(errno) << std::endl;
        std::string msg = strerror(errno) + std::string(" - try to run executable from project root specifying the path './build.drive'");
        throw std::system_error(errno, std::generic_category(), msg.c_str());
    }

    if (!reader.parse(jsonMap, root))
    {
        std::cerr << reader.getFormattedErrorMessages() << std::endl;
        throw std::system_error(-1, std::generic_category(), reader.getFormatedErrorMessages());
    }

    this->size_ = root["NOD"].size();
    for (int i = 0; i < this->size_; i++)
    {
        // Node *n = new Node(root["NOD"][i]);
        // std::shared_ptr<Node> n(new Node(root["NOD"][i]));
        NODE_PTR n = std::make_shared<Node>(root["NOD"][i]);
        this->nodesSet[n->name()] = n;
        this->coordMap[n->coord()].push_back(n->name());
    }

    this->linkNodes();
}

// Map::Map(const Map& obj)
// {
//     this->size_ = obj.size_;
//     this->nodesSet = obj.nodesSet;
//     this->goal = obj.goal;
//     this->start = obj.start;
// }

// Map& Map::operator=(const Map& obj)
// {
//     this->size_ = obj.size_;
//     this->nodesSet = obj.nodesSet;
//     this->goal = obj.goal;
//     this->start = obj.start;
//     return *this;
// }

void Map::linkNodes()
{
    // Dictionary::iterator node;
    // for ( node = this->nodesSet.begin(); node != this->nodesSet.end(); node++ )
    for (auto & node : this->nodesSet)
    {
        if (!node.second->linked())
        {    
            node.second->inBack      = (node.second->arcs()[IN_BACK] != "none") ? this->nodesSet[node.second->arcs()[IN_BACK]] : NULL;
            node.second->inRight     = (node.second->arcs()[IN_RIGHT] != "none") ? this->nodesSet[node.second->arcs()[IN_RIGHT]] : NULL;
            node.second->inAhead     = (node.second->arcs()[IN_AHEAD] != "none") ? this->nodesSet[node.second->arcs()[IN_AHEAD]] : NULL;
            node.second->inLeft      = (node.second->arcs()[IN_LEFT] != "none") ? this->nodesSet[node.second->arcs()[IN_LEFT]] : NULL;
            node.second->outBack     = (node.second->arcs()[OUT_BACK] != "none") ? this->nodesSet[node.second->arcs()[OUT_BACK]] : NULL;
            node.second->outRight    = (node.second->arcs()[OUT_RIGHT] != "none") ? this->nodesSet[node.second->arcs()[OUT_RIGHT]] : NULL;
            node.second->outAhead    = (node.second->arcs()[OUT_AHEAD] != "none") ? this->nodesSet[node.second->arcs()[OUT_AHEAD]] : NULL;
            node.second->outLeft     = (node.second->arcs()[OUT_LEFT] != "none") ? this->nodesSet[node.second->arcs()[OUT_LEFT]] : NULL;
        }

        if (node.second->outBack != NULL &&
            !this->edgeExist(node.second->name(), node.second->outBack->name()))
        {
            this->addEdge(node.second, node.second->outBack, 1);
            // std::cout << this->edge(node.second->name(), node.second->outBack->name()) << std::endl;
        }
        if (node.second->outRight != NULL &&
            !this->edgeExist(node.second->name(), node.second->outRight->name()))
        {
            this->addEdge(node.second, node.second->outRight, 1);
            // std::cout << this->edge(node.second->name(), node.second->outRight->name()) << std::endl;
        }
        if (node.second->outAhead != NULL &&
            !this->edgeExist(node.second->name(), node.second->outAhead->name()))
        {
            this->addEdge(node.second, node.second->outAhead, 1);
            // std::cout << this->edge(node.second->name(), node.second->outAhead->name()) << std::endl;
        }
        if (node.second->outLeft != NULL &&
            !this->edgeExist(node.second->name(), node.second->outLeft->name()))
        {    
            this->addEdge(node.second, node.second->outLeft, 1);
            // std::cout << this->edge(node.second->name(), node.second->outLeft->name()) << std::endl;
        }
        node.second->setLinked();
    }
}

void Map::setGoal(std::string nodeName)     { this->goal = this->nodesSet[nodeName]; }
void Map::setStart(std::string nodeName)    { this->start = this->nodesSet[nodeName]; }

// TODO: remove commented
// Node Map::operator[] (const std::string nodeName)
// {
//     return *this->nodesSet[nodeName];
// }

// NodesVect Map::operator[] (const std::complex<double> coord)
// {
//     NodesVect nodes;
//     for (auto &nodeName : this->coordMap[coord])
//     {
//         nodes.push_back(this->nodesSet[nodeName]);
//     }
//     if (nodes.size() > 0)   { return nodes; }
//     else
//     {
//         throw std::system_error(-1, std::generic_category(), "there is no Node at the specified coordinate");
//     }

// }

Map& Map::getInstance()
{
    // static Map instance("/home/mihai/Workspace/BOSCH_2019/Holistic2_v2/master/resources/Map.json");
    static Map instance("resources/Map.json");
    return instance;
}