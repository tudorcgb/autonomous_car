#ifndef IMU_H
#define IMU_H

#include "RTIMULib.h"
#include "observer.h"
#include <thread>

class IMU : public Subject
{
public:
    IMU();
    IMU(const std::string configPath, const std::string option);
    IMU(const IMU& imu);
    IMU& operator=(const IMU& imu);
    ~IMU();
    void readIMU();
    void initIMU();
    void setAngleToZero();
    float yaw();
    void start();
    void stop();
private:
    RTIMU *imu;
    bool running;
    uint64_t rateTimer;
    float initial_angle_yaw;
    float curent_yaw_rad;
    pthread_mutex_t lock_yaw;
    std::thread readingThread;
    uint64_t displayTimer;
};

#endif // IMU_H