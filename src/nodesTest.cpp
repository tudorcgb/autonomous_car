#if 0

#include "pathtracking.h"

int main(int argc, char** argv)
{
    PathTracking p("NOD112", "NOD117");
    std::cout << "Current pos: " << p.globalPosition().getPosition() << std::endl;
    // std::cout << "Points: " << p.nextNode() << std::endl;
    std::vector<std::complex<double>> vect;
    // p.nextNodes(10, vect);
    std::vector<std::complex<double>> nodes;
    int n = p.nextObstaclesNodes(std::complex<double>(8.1,1.8), 3, nodes);
    // for (int i = 0; i < 10; i++)
    // {
    //     std::cout << vect[i] << std::endl;
    // }

    for (int i = 0; i < n; i++)
    {
        std::cout << nodes[i] << std::endl;
    }

    // std::cout << "Obst: " << nodes[0] << std::endl;
    return 0;
}

#endif