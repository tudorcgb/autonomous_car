#ifndef GPS_H
#define GPS_H

#include "observer.h"
#include "gpsconnection.h"

class GPS : public Subject
{
public:
    GPS(int carID, uint16_t negotiationPort);
    void start();
    void stop();
private:
    GPSConnection connection;
    GPSData potision;
    int carID;
    uint16_t negotiationPort;
};

#endif // GPS_H