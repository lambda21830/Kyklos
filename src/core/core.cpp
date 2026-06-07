#include "core.hpp"
#include <cmath>
#include <vector>
#include <utility>

bool areAligned(const std::vector<Point> &points)
{
    double determinant = points[0].x * (points[1].y - points[2].y) + points[1].x * (points[2].y - points[0].y) + points[2].x * (points[0].y - points[1].y);
    return std::abs(determinant) < EPS;
}

CalculationResult threePoints(const std::vector<Point> &points)
{
    if (areAligned(points))
    {
        return CalculationResult{{}, Status::POINTS_ALIGNED};
    }

    // Composizione matrice aumentata
    std::vector<std::vector<double>> matrix(3, std::vector<double>(4));
    for (int i = 0; i < 3; i++)
    {
        double x = points[i].x;
        double y = points[i].y;

        matrix[i][0] = x;
        matrix[i][1] = y;
        matrix[i][2] = 1;
        matrix[i][3] = -(x * x + y * y);
    }

    // Pivoting parziale
    for (int i = 0; i < 3; i++)
    {
        int maxPivotRow = i;
        for (int j = i + 1; j < 3; j++)
        {
            if (std::abs(matrix[j][i]) > std::abs(matrix[maxPivotRow][i]))
            {
                maxPivotRow = j;
            }
        }

        if (maxPivotRow != i)
        {
            std::swap(matrix[i], matrix[maxPivotRow]);
        }

        // Controllo singolarità matrice
        if (std::abs(matrix[i][i]) < EPS)
        {
            return CalculationResult{{}, Status::SINGULAR_MATRIX};
        }

        // Eliminazione gaussiana
        for (int j = i + 1; j < 3; j++)
        {
            double mult = matrix[j][i] / matrix[i][i];
            for (int k = i; k < 4; k++)
            {
                matrix[j][k] -= matrix[i][k] * mult;
            }
        }
    }

    // Back substitution
    std::vector<double> solutionValues(3);
    for (int i = 2; i >= 0; i--)
    {
        double known = matrix[i][3];
        for (int j = i + 1; j < 3; j++)
        {
            known -= matrix[i][j] * solutionValues[j];
        }
        solutionValues[i] = known / matrix[i][i];
    }

    return CalculationResult{solutionValues, Status::SUCCESS};
}

std::vector<double> centerPoint(const Point &center, const Point &point)
{
    std::vector<double> coefficients(3);

    coefficients[0] = -2 * center.x;
    coefficients[1] = -2 * center.y;
    double radiusSquare = std::pow((point.x - center.x), 2) + std::pow((point.y - center.y), 2);
    coefficients[2] = std::pow(center.x, 2) + std::pow(center.y, 2) - radiusSquare;

    return coefficients;
}

std::vector<double> centerRadius(const Point &center, const double radius)
{
    if (radius < 0)
    {
        return std::vector<double>{};
    }

    std::vector<double> coefficients(3);
    coefficients[0] = -2 * center.x;
    coefficients[1] = -2 * center.y;
    coefficients[2] = std::pow(center.x, 2) + std::pow(center.y, 2) - std::pow(radius, 2);

    return coefficients;
}