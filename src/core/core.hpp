#ifndef CORE_HPP
#define CORE_HPP

#define EPS 1e-9

#include "data-types.hpp"

bool areAligned(const std::vector<Point> &points);
CalculationResult threePoints(const std::vector<Point> &points);
std::vector<double> centerPoint(const Point &center, const Point &point);
std::vector<double> centerRadius(const Point &center, const double radius);

#endif