#ifndef IGEOMPRIM_H
#define IGEOMPRIM_H

#include <complex>

class IGeomPrim
{
public:
    // virtual double dot(const IGeomPrim& b) = 0;
    virtual double distance(const std::complex<double>& point) = 0;
    virtual double direction(const std::complex<double>& vect) = 0;
    std::complex<double> A;
    std::complex<double> B;
    double length;
};

#endif // IGEOMPRIM_H