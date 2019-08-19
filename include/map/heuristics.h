#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <iostream>
#include <complex>

float manhattanHeuristic(std::complex<double> p0, std::complex<double> p1);

float euclideanHeuristic(std::complex<double> p0, std::complex<double> p1);

#endif // HEURISTICS_H