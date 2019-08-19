#if 0

#include "imagecoord.h"

int main(int argc, char** argv)
{
    ImageCoord image = ImageCoord(23, 23, 800, 400, 45);
    std::cout << image.toImageCoord(0.7, 0.2) << std::endl;
    return 0;
}

#endif