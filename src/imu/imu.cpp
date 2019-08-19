#include "imu.h"

#define PI 3.14159265

IMU::IMU()
{

}

IMU::IMU(const std::string configPath, const std::string option)
{
    RTIMUSettings *settings = new RTIMUSettings(configPath.c_str(), option.c_str());

    this->imu = RTIMU::createIMU(settings);

    std::cout << "beforeNULL" << std::endl;
    if ((this->imu == NULL) || (this->imu->IMUType() == RTIMU_TYPE_NULL))
    {
        std::cerr << "No IMU found" << std::endl;
        exit(-1);
    }

    this->initial_angle_yaw = 0.0;
    this->curent_yaw_rad = 0.0;

    std::cout << "beforeInit" << std::endl;
    this->imu->IMUInit();
    std::cout << "afterInit" << std::endl;
    this->running = false;
    this->imu->setSlerpPower(0);
    std::cout << "afterPower" << std::endl;
    this->imu->setGyroEnable(true);
    std::cout << "afterGyro" << std::endl;
    this->imu->setAccelEnable(false);
    std::cout << "afterAccel" << std::endl;
    this->imu->setCompassEnable(true);
    std::cout << "afterCompass" << std::endl;

    rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();

    usleep(imu->IMUGetPollInterval() * 1000);
    setAngleToZero();

    if (pthread_mutex_init(&lock_yaw, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        exit(-1); 
    } 
    
    // free(settings);
    this->className_ = "IMU";
    this->start();
}

IMU::~IMU() {}

void IMU::setAngleToZero()
{
    this->imu->IMURead();
    RTIMU_DATA imuData = imu->getIMUData();
    float theta = (float)imuData.fusionPose.z();
    this->initial_angle_yaw = theta;
    printf("initial angle:%f\n",initial_angle_yaw);
}

void IMU::readIMU()
{
    while(this->running){
        usleep(imu->IMUGetPollInterval() * 100);
        
        imu->IMURead();
        RTIMU_DATA imuData = imu->getIMUData();
        float theta = (float)imuData.fusionPose.z();
         theta = theta - initial_angle_yaw;
            if(theta < -PI){
                theta = 2*PI - abs(theta);                
            }else if(theta > PI){
                theta = -(2*PI - abs(theta));
            }
        pthread_mutex_lock(&lock_yaw);
            this->curent_yaw_rad = fmod(theta,PI);
        pthread_mutex_unlock(&lock_yaw); 
        this->notifyObservers();
    }
}

float IMU::yaw()
{
    float value;
    pthread_mutex_lock(&lock_yaw);
        value = this->curent_yaw_rad;
    pthread_mutex_unlock(&lock_yaw);
    return value;
}

void IMU::start()
{
    this->running = true;
    this->readingThread = std::thread(&IMU::readIMU, this);
}

void IMU::stop()
{
    this->running = false;
    this->readingThread.join();
}