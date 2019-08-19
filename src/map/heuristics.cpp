#ifndef HEURISTICS_H
#include "heuristics.h"

float manhattanHeuristic(std::complex<double> p0, std::complex<double> p1)
{
    return abs(p0.real() - p1.real()) + abs(p0.imag() - p1.imag());
}

float euclideanHeuristic(std::complex<double> p0, std::complex<double> p1)
{
    return sqrt( 
        (p1.real()-p0.real()) * (p1.real()-p0.real()) + 
        (p1.imag()-p0.imag()) * (p1.imag()-p0.imag())
    );
}

#endif // HEURISTICS_H