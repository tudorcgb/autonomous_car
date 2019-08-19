#include "vec2.h"

double vecLength(const std::complex<double>& vect);

Vec2::Vec2(std::complex<double> a, std::complex<double> b)
{
    this->A = a;
    this->B = b;
    double real = this->B.real() - this->A.real();
    double imag = this->B.imag() - this->A.imag();
    this->orientation = std::complex<double>(real, imag);
    this->length = fabs(sqrt(orientation.real()*orientation.real() + orientation.imag()*orientation.imag()));

    // std::cout   << "A: " 
    //             << this->A 
    //             << " B: " 
    //             << this->B 
    //             << " orientation: "
    //             << this->orientation
    //             << " length: "
    //             << this->length
    //             << std::endl;
    
    if (orientation.real() != 0)
    {
        double nx = -orientation.imag()/orientation.real();
        this->normal_ = std::complex<double>(nx, 1.0);
        this->normal_ /= vecLength(this->normal_);
    }
    else if (orientation.imag() != 0)
    {
        double ny = -orientation.real()/orientation.imag();
        this->normal_ = std::complex<double>(1.0, ny);
        this->normal_ /= vecLength(this->normal_);
    }
    else
    {
        this->normal_ = std::complex<double>(0.0, 0.0);
    }

    // std::cout   << "normal: "
    //             << this->normal_
    //             << std::endl;
}

Vec2::Vec2(const Vec2& v)
{
    this->A = v.A;
    this->B = v.B;
    this->length = v.length;
    this->orientation = v.orientation;
}

Vec2& Vec2::operator=(const Vec2& v)
{
    this->A = v.A;
    this->B = v.B;
    this->length = v.length;
    this->orientation = v.orientation;
    return *this;
}

const std::complex<double>& Vec2::normal() const    { return this->normal_; }

double dot(const std::complex<double>& a, const std::complex<double>& b)
{
    return (a.real() * b.real()) + (a.imag() * b.imag());
}

double Vec2::distance(const std::complex<double>& point)
{
    std::complex<double> normPoint = point - this->A;
    std::complex<double> vect = std::complex<double>(normPoint.real() - this->A.real(), normPoint.imag() - this->A.imag());
    double normalLength = fabs(
        sqrt(
            (double)(this->normal_.real()*this->normal_.real()) + 
            (double)(this->normal_.imag()*this->normal_.imag())
        )
    );
    // std::cout   << "normal: "
    //             << this->normal_
    //             << " length: "
    //             << normalLength
    //             << " vect: "
    //             << vect
    //             << std::endl;
    return dot(vect, normal_)/normalLength;
}

double vecLength(const std::complex<double>& vect)
{
    return fabs(
        sqrt(
            (double)(vect.real()*vect.real()) + 
            (double)(vect.imag()*vect.imag())
        )
    );
}

// double Vec2::vectPointSin(const std::complex<double>& point)
// {
//     std::complex<double> ipot = std::complex<double>(point.real() - this->A.real(), point.imag() - this->A.imag());
//     // std::complex<double> cat = dot(ipot,this->normal_/vecLength(this->normal_));
//     // std::complex<double> cat = ipot - (this->normal_ * this->distance(point));
//     // std::complex<double> cat = (dot(ipot, this->orientation)/(length*length))*orientation;
//     // std::cout << distance(point) << std::endl;
//     std::cout   << "ipot: " << ipot
//                 << " cat: " << cat
//                 << std::endl;
//     double ipotLength = vecLength(ipot);
//     double catLength = vecLength(cat);
//     if (ipotLength == 0)    { return 0.0; }
//     return catLength/ipotLength;
// }

double Vec2::cross(const std::complex<double>& vect)
{
    // std::cout << this->orientation << std::endl;
    return this->orientation.real()*vect.imag() - this->orientation.imag()*vect.real();
}

double Vec2::direction(const std::complex<double>& point)
{
    std::complex<double> normPoint  = point - this->A;  // TODO: better aproach
    return this->cross(normPoint)/this->length;
}

std::complex<double> Vec2::project(const std::complex<double>& vect)
{
    std::complex<double> normVect  = vect - this->A;   // TODO: better aproach
    std::complex<double> pos = (dot(normVect, this->orientation)/(this->length*this->length))*orientation + this->A;   // TODO: better aproach withput +this->A
    if ((vecLength(pos - this->A)) <= this->length)
    {
        return pos;   
    }
    return this->B;
}