#ifndef CLI_HPP
#define CLI_HPP

#define EPS 1e-9

// Text Colors
#define NC "\033[0m" // No Color
#define RED "\033[31m"
#define GREEN "\033[32m"

#include <string>
#include <sstream>
#include <vector>

std::string getOS();
void clearBuffer();
void clearScreen();
void pressAKey();
void dots();
std::string formatNumber(const double value);
void appendTerm(std::ostringstream &equation, const double coefficient, const std::string &var);
std::string composeEquation(const std::vector<double> coefficients);
void handleThreePoints();
void handleCenterPoint();
void handleCenterRadius();
void displayTitle();
void displayMenu();
int getMenuChoice();
void runMenuLoop();

#endif