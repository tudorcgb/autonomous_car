#ifndef GPSDATA_G
#include "gpsdata.h"
#endif

GPSData::GPSData(const int id /*= 0*/, const std::complex<double> position /*= 0.+0i*/, std::complex<double> orientation/* = 0.+0i*/) /*:
    id_(id),
    position_(position),
    orientation_(orientation)*/
{
    this->id = id;
    this->position = position;
    this->orientation = orientation;
    this->updateTime = std::time(nullptr);
}

GPSData::GPSData(const GPSData &obj)
{
    this->id = obj.id;
    this->position = obj.position;
    this->orientation = obj.orientation;
    this->updateTime = obj.updateTime;
}

GPSData& GPSData::operator=(const GPSData &obj)
{
    this->id = obj.id;
    this->position = obj.position;
    this->orientation = obj.orientation;
    this->updateTime = obj.updateTime;
    return *this;
}

std::ostream& operator<< (std::ostream& stream, const GPSData& gpsData)
{
    return stream << "id: "
                  << gpsData.id
                  << " Pos: "
                  << gpsData.position
                  << " Orient: "
                  << gpsData.orientation;
}

int GPSData::getId()
{
    return this->id;
}

std::complex<double> GPSData::getPosition()
{
    return this->position;
}

std::complex<double> GPSData::getOrientation()
{
    return this->orientation;
}

std::time_t GPSData::getLastUpdateTime()
{
    return this->updateTime;
}

void GPSData::update(const std::complex<double> position, std::complex<double> orientation)
{
    this->position = position;
    this-> orientation = orientation;
    this->updateTime = std::time(nullptr);
}


