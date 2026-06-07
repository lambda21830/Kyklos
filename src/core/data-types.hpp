#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <vector>

struct Point
{
    double x, y;
};

enum class Status
{
    SUCCESS,
    POINTS_ALIGNED,
    SINGULAR_MATRIX
};

struct CalculationResult
{
    std::vector<double> coefficients;
    Status status;
};

#endif