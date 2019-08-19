#if 0

#include "imu.h"
#include "imuencoderposition.h"
#include "connexc.h"

int main()
{
    IMU imu("/media/pi/PiDrive1/Holistic2_v4/master/lib/imu/","RTIMULib");
    CarControl c;
    IMUEncoderPosition positionSystem(&c);
    imu.attachObserver(&positionSystem);
    sleep(2);
    imu.start();
    try
    {
        c.enablePID();
        c.move(0.2, 0.0);
        sleep(2);
        c.brake(0.0);
        sleep(2);
    }
    catch (ConnectionException e)
    {
        std::cerr << e.what() << std::endl;
    }
    // sleep(100);
    imu.stop();
    std::cout << "IMU stop" << std::endl;

    return 0;
}

#endif
