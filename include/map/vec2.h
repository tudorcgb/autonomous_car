#ifndef VEC2_H
#define VEC2_H

#include <iostream>

#ifndef IGEOMPRIM_H
#include "igeomprim.h"
#endif // IGEOMPRIM_H

/*!
* \name vecLength
* \brief   Returns the length or norm of a vector.
*          |v| = sqrt(v.x^2 + v.y^2)
* \param vect 
* \return double 
*/
double vecLength(const std::complex<double>& vect);

class Vec2 : public IGeomPrim
{
public:
    /*!
     * \name Vec2
     * \brief Construct a new Vec 2 object.
     * \param a     Origin point
     * \param b     Destination point
     */
    Vec2(const std::complex<double> a, const std::complex<double> b);

    /*!
     * \name Vec2
     * \brief Copy constructor.
     * 
     * \param v 
     */
    Vec2(const Vec2& v);

    /*!
     * \name operator=
     * \brief Assignment operator..
     * \param v     object to be copied
     * \return Vec2& 
     */
    Vec2& operator=(const Vec2& v);

    /*!
     * \name distance
     * \brief   Compute distance from a poin to a vector.
     * \param point
     * \return double 
     */
    double distance(const std::complex<double>& point);

    /*!
     * \name normal
     * \brief   Getter for normal orthogonal vector.
     * \return const std::complex<double>& 
     */
    const std::complex<double>& normal() const;

    /*!
     * \name dot
     * \brief   Compute dot product between 2 vectors.
     *          (a.x * b.x) + (a.y * b.y)
     * \param a     First vector
     * \param b     Second vector
     * \return double 
     */
    friend double dot(const std::complex<double>& a, const std::complex<double>& b);

    // /*!
    //  * \name vecLength
    //  * \brief   Returns the length or norm of a vector.
    //  *          |v| = sqrt(v.x^2 + v.y^2)
    //  * \param vect 
    //  * \return double 
    //  */
    // friend double vecLength(const std::complex<double>& vect);
    // double vectPointSin(const std::complex<double>& point);

    /*!
     * \name cross
     * \brief   Compute cross product in 2D using the formula:
     *          cross = (v1.x * v2.y) - (v1.y * v2.x)
     * \param vect 
     * \return double 
     */
    double cross(const std::complex<double>& vect);

    /*!
     * \name direction
     * \brief   Returns distance between point an vector if the point is in the left of
     *          the vector or -distance if the point is in the right of the vector.
     * \param vect      a vector or a point
     * \return double 
     */
    double direction(const std::complex<double>& point);

    /*!
     * \name project
     * \brief Returns the projectet vector on this
     * \param vect 
     * \return std::complex<double> 
     */
    std::complex<double> project(const std::complex<double>& vect);

    std::complex<double> orientation;
    // std::complex<double> vec;
private:
    std::complex<double> normal_;
};

#endif // VEC2_H