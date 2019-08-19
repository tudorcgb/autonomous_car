#ifndef POSITIONDRIVE_H
#define POSITIONDRIVE_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pthread.h>

#include "observer.h"
#include "carcontrol.h"
#include "pathtracking.h"
#include "imuencoderposition.h"
#include "connexc.h"

class PositionDrive : public IObserver
{
public:
    PositionDrive();
    PositionDrive(const CarControl* c, const PathTracking* path_);
    ~PositionDrive();
    // PositionDrive(const PositionDrive& obj);
    PositionDrive& operator=(const PositionDrive& obj);
    void drive(std::vector<cv::Point> &dstPoints);
    void drivePoints(const int pointsNo);
    void drivePointsObs(std::vector<std::complex<double>> &pointsVector);
    void update(Subject* subject) override;
    int nsleep(long miliseconds);
    float angle;
    pthread_mutex_t lock_angle;
    bool done;
private:
    CarControl *c;
    PathTracking* path;
    float x;
    float y;
    float curent_yaw_rad;
};

#endif // POSITIONDRIVE_H