#include "cli.hpp"
#include <iostream>
#include <limits>
#include <ios>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include "../core/data-types.hpp"
#include "../core/core.hpp"

std::string getOS()
{
#ifdef _WIN32
    return "Windows";
#elif __APPLE__ || __MACH__
    return "Apple";
#elif __linux__
    return "Linux";
#else
    return "Other";
#endif
}

void clearBuffer()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearScreen()
{
    getOS() == "Windows" ? std::system("cls") : std::system("clear");
}

void pressAKey()
{
    if (getOS() == "Windows")
    {
        std::system("pause");
        clearScreen();
    }

    else
    {
        std::cout << "Premi un tasto per continuare..." << std::flush;
        std::system("read -sn 1");
        clearScreen();
    }
}

void dots()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    for (int i = 0; i < 3; i++)
    {
        std::cout << '.' << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    std::cout << std::endl;
}

std::string formatNumber(const double value)
{
    std::ostringstream ss;
    ss.setf(std::ios::fixed);
    ss.precision(3);
    ss << value;
    return ss.str();
}

void appendTerm(std::ostringstream &equation, const double coefficient, const std::string &var)
{
    // Salta i termini ~ 0
    if (std::abs(coefficient) < EPS)
    {
        return;
    }

    // Termini positivi preceduti da '+'
    if (coefficient > 0)
    {
        if (var.empty())
        {
            equation << "+" << formatNumber(coefficient);
        }

        else
        {
            // Se il coefficiente ~ 1, +1x -> +x
            if (std::abs(coefficient - 1.0) < EPS)
            {
                equation << "+" << var;
            }

            else
            {
                equation << "+" << formatNumber(coefficient) << var;
            }
        }
    }

    // Termini negativi preceduti da '-'
    else
    {
        if (var.empty())
        {
            equation << formatNumber(coefficient);
        }

        else
        {
            // Se il coefficiente ~ -1, -1x -> -x
            if (std::abs(coefficient + 1.0) < EPS)
            {
                equation << "-" << var;
            }

            else
            {
                equation << formatNumber(coefficient) << var;
            }
        }
    }
}

std::string composeEquation(const std::vector<double> coefficients)
{
    // Setup iniziale equazione
    std::ostringstream equation;
    equation.setf(std::ios::fixed);
    equation.precision(3);
    equation << "x²+y²";

    // Composizione equazione con termini non nulli
    appendTerm(equation, coefficients[0], "x");
    appendTerm(equation, coefficients[1], "y");
    appendTerm(equation, coefficients[2], ""); // Aggiunta termine noto
    std::string equation_str = equation.str();
    equation_str += " = 0";

    return equation_str;
}

void handleThreePoints()
{
    // Inserimento coordinate punti
    std::vector<Point> points(3);
    for (int i = 0; i < 3; i++)
    {
        std::cout << "Coordinate del punto " << i + 1 << " (x, y): ";
        std::cin >> points[i].x >> points[i].y;
    }

    // Calcolo e gestione risultati
    std::cout << "Calcolo in corso..." << std::endl;
    auto result = threePoints(points);
    switch (result.status)
    {
    case Status::SUCCESS:
        std::cout << GREEN << "Soluzione trovata!" << NC << std::endl;
        std::cout << composeEquation(result.coefficients) << std::endl;
        break;

    case Status::POINTS_ALIGNED:
        std::cerr << RED << "Errore: " << NC << "I 3 punti sono allineati" << std::endl;
        break;

    case Status::SINGULAR_MATRIX:
        std::cerr << RED << "Errore: " << NC "La matrice è singolare, impossibile calcolare una soluzione" << std::endl;
        break;
    }
}

void handleCenterPoint()
{
    // Inserimento coordinate punti
    Point center, point;
    std::cout << "Coordinate del centro (x, y): ";
    std::cin >> center.x >> center.y;
    std::cout << "Coordinate del punto P (x, y): ";
    std::cin >> point.x >> point.y;

    // Visualizzazione equazione
    std::vector<double> coefficients = centerPoint(center, point);
    std::cout << GREEN << "Soluzione trovata!" << NC << std::endl;
    std::cout << composeEquation(coefficients) << std::endl;
}

void handleCenterRadius()
{
    // Inserimento coordinate centro e misura raggio
    Point center;
    double radius;
    std::cout << "Coordinate del centro (x, y): ";
    std::cin >> center.x >> center.y;
    std::cout << "Misura del raggio: ";
    std::cin >> radius;

    if (radius < 0)
    {
        std::cerr << RED << "Errore: " << NC << "Il raggio è minore di 0" << std::endl;
        return;
    }

    // Visualizzazione equazione
    std::vector<double> coefficients = centerRadius(center, radius);
    std::cout << GREEN << "Soluzione trovata!" << NC << std::endl;
    std::cout << composeEquation(coefficients) << std::endl;
}

void displayTitle()
{
    std::cout << "=============================\n";
    std::cout << "           KYKLOS          \n";
    std::cout << "=============================\n";
    std::cout << "Calcolatrice di Circonferenze\n\n";
}

void displayMenu()
{
    displayTitle();

    std::cout << "1. Circonferenza dati 3 punti\n";
    std::cout << "2. Circonferenza dato il centro e un punto\n";
    std::cout << "3. Circonferenza dato il centro e il raggio\n";
    std::cout << "4. Esci\n";
    std::cout << "Seleziona un'opzione: ";
}

int getMenuChoice()
{
    int choice;
    while (true)
    {
        displayMenu();
        if (std::cin >> choice)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice >= 1 && choice <= 4)
            {
                clearScreen();
                return choice;
            }
        }

        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cerr << RED << "Errore: " << NC << "Inserisci un numero tra 1 e 4\n";
        pressAKey();
    }
}

void runMenuLoop()
{
    bool running = true;
    while (running)
    {
        int choice = getMenuChoice();
        switch (choice)
        {
        case 1:
            handleThreePoints();
            pressAKey();
            break;
        case 2:
            handleCenterPoint();
            pressAKey();
            break;
        case 3:
            handleCenterRadius();
            pressAKey();
            break;
        case 4:
            std::cout << "Uscita in corso" << std::flush;
            dots();
            running = false;
            break;
        }
    }
}