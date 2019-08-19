#include "imuencoderposition.h"
#include "connexc.h"
#include "gpsconnection.h"

IMUEncoderPosition::IMUEncoderPosition() {}

IMUEncoderPosition::IMUEncoderPosition(const CarControl* car)
{
    // this->carControl = car;
    // try
    // {
    //     car->enableEncoder(std::bind(&IMUEncoderPosition::readEncoder, this, std::placeholders::_1));
    //     std::cout << "encoder enabled" << std::endl;
    // }
    // catch (ConnectionException e)
    // {
    //     std::cout << e.what() << std::endl;
    // }

    this->initial = true;
    this->x = 0.0; // todo get from gps
    this->y = 0.0; // todo get from gps

    this->xAnt = 30;
    this->yAnt = 71.6;
    this->readingCounter = 0;

    this->className_ = "IMUEncoderPosition";
    if (pthread_mutex_init(&lock_XY, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        exit(-1);
    }
}

IMUEncoderPosition::~IMUEncoderPosition() {}

void* IMUEncoderPosition::readEncoder(void* str)
{
    //  display 10 times per second 198906242
    float local_yaw = 0.0;
    pthread_mutex_lock(&lock_XY);
        local_yaw = curent_yaw_rad;
    pthread_mutex_unlock(&lock_XY); 
        double flt = atof( (char*)str);
        double du = (flt / ROT_PER_METER);
        float locx = xAnt + (du * (float)cos(local_yaw));
        float locy = yAnt + (du * (float)sin(local_yaw)); // + sin.. for y growind downwords

    pthread_mutex_lock(&lock_XY);
        x = locx;
        y = locy;
        xAnt = x;
        yAnt = y;
    pthread_mutex_unlock(&lock_XY); 
        readingCounter++;
        if(readingCounter == 20){            
            // storePoint(x, y);
            points.push_back(cv::Point(x,y));
            readingCounter = 0;
        }
    this->notifyObservers();
}

float IMUEncoderPosition::getX()
{
    float value;
    pthread_mutex_lock(&lock_XY);
        value = x;
    pthread_mutex_unlock(&lock_XY);
    return value;
}

float IMUEncoderPosition::getY()
{
    float value;
    pthread_mutex_lock(&lock_XY);
        value = y;
    pthread_mutex_unlock(&lock_XY);
    return value;
}

float IMUEncoderPosition::yaw()
{
    float value;
    pthread_mutex_lock(&lock_XY);
        value = curent_yaw_rad;
    pthread_mutex_unlock(&lock_XY);
    return value;
}

void IMUEncoderPosition::update(Subject* subject)
{
    if (subject->className() == "IMU")
    {
        this->curent_yaw_rad = ((IMU*)subject)->yaw();
    }
    else if (subject->className() == "GPSConnection")
    {
        std::cout << "\t\tFromGPS" << std::endl;
        this->x = ((GPSConnection*)subject)->position.getPosition().real()*100.0 + 27.0;
        this->y = (double)521.5 - ((GPSConnection*)subject)->position.getPosition().imag()*100.0;

        if (this->initial)
        {
            std::cout << "setare GPS: " << this->x << ", " << this->y << std::endl;
            this->xAnt = this->x;
            this->yAnt = this->y; 
            this->initial = false;
        }
    }
}