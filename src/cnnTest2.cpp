#if 0

#include "caffenetwork.h"
#include "image.h"
#include "timer.h"

#include "carcontrol.h"

int main()
{
    CaffeNetwork caffe("resources/caffemodel/model_test_small.prototxt", "resources/caffemodel/CAR_iter_4000.caffemodel");
    Image img;
    Timer t("t", 50000, std::bind(&Image::nextFrame, &img, (void*)(NULL)));
    t.start();

    // Timer inference("t", 50000, std::bind(&Caffe::infer, &img, (void*)(NULL)));
    // inference.start();

    CarControl c;
    img.attachObserver(&caffe);
    c.brake(0.0);
    c.enablePID();
    sleep(1.0);
    while (true)
    {
        c.move(0.2, 23.0*(2.0*caffe.infer()-1.0));
    }

    sleep(3);

    c.brake(0.0);
    return 0;
}

#endif