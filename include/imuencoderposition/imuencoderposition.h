#ifndef IMUENCODERPOSITION_H
#define IMUENCODERPOSITION_H

#include <functional>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "observer.h"
#include "carcontrol.h"
#include "imu.h"

#ifndef ROT_PER_METER
#define ROT_PER_METER 156.2   // should be 148
#endif // ROT_PER_METER

class IMUEncoderPosition : public Subject, public IObserver
{
public:
    IMUEncoderPosition();
    IMUEncoderPosition(const CarControl* car);
    ~IMUEncoderPosition();
    IMUEncoderPosition(const IMUEncoderPosition& e);
    IMUEncoderPosition& operator=(const IMUEncoderPosition& e);
    void* readEncoder(void* str);
    void update(Subject* subject) override;
    float getX();
    float getY();
    float yaw();
private:
    CarControl* carControl;
    int readingCounter;
    float x;
    float y;
    float xAnt;
    float yAnt;
    float curent_yaw_rad;
    pthread_mutex_t lock_XY;
    std::vector<cv::Point> points;
    bool initial;
};

#endif // IMUENCODERPOSITION_H