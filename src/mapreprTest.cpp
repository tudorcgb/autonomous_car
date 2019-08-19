#if 0

#include "maprepresentation.h"
#include "path.h"

int main(int argc, char** argv)
{
    // Path p("NOD1", "NOD133");
    Path p("NOD99", "NOD100");
    MapRepresentation m("resources/Race_resize_mark_center.png", 23, 23, 834, 518, 45);
    m.addPrimitives(p.getEdgesInPath());
    m.drawAll();
    m.setMouseCallbackParam(&p);
    m.showMap();
    std::cout << m.getPosition() << std::endl;
    cv::waitKey();
    std::cout << m.getPosition() << std::endl;
    

    return 0;
}

#endif