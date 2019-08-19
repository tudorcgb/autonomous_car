#if 0

#include "positiondrive.h"


int main()
{
    IMU imu("/media/pi/PiDrive1/Holistic2_v4/master/lib/imu/","RTIMULib");
    imu.start();
    CarControl c;
    IMUEncoderPosition positionSystem(&c);
    c.enableEncoder(std::bind(&IMUEncoderPosition::readEncoder, &positionSystem, std::placeholders::_1));
    imu.attachObserver(&positionSystem);
    PathTracking p("NOD1", "NOD45");
    PositionDrive dr(&c, &p);
    positionSystem.attachObserver(&dr);
    sleep(2);
    try
    {
        c.enablePID();
        sleep(2);
    }
    catch (ConnectionException e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::thread run(&PositionDrive::drivePoints, &dr, 15);
    while(true)
    {
        pthread_mutex_lock(&dr.lock_angle);
        c.move(0.2, dr.angle);
        pthread_mutex_unlock(&dr.lock_angle);
        dr.nsleep(100);
    }

    run.join();
    imu.stop();


    return 0;
}

#endif
