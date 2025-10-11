#include <cmath>
#include <cstdlib>

#include <chrono>
#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Text Colors
#define NC "\033[0m" // No Color
#define RED "\033[31m"
#define GREEN "\033[32m"

struct Point
{
    double x, y;
};

string get_os()
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

void clear_buffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clear_screen()
{
    get_os() == "Windows" ? system("cls") : system("clear");
}

void press_a_key_to_continue()
{
    if (get_os() == "Windows")
    {
        system("pause");
        clear_screen();
    }

    else
    {
        cout << "Premi un tasto per continuare..." << flush;
        system("read -sn 1");
        clear_screen();
    }
}

void dots()
{
    this_thread::sleep_for(chrono::milliseconds(300));

    for (int i = 0; i < 3; i++)
    {
        cout << '.' << flush;
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << endl;
}

bool are_aligned(Point p1, Point p2, Point p3)
{
    double determinant = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y);
    const double epsilon = 1e-9;

    return abs(determinant) < epsilon;
}

string format_number(double value)
{
    ostringstream ss;

    ss.setf(ios::fixed);
    ss.precision(3);
    ss << value;

    return ss.str();
}

void append_term(ostringstream &equation, double coefficient, const string &var, double epsilon)
{
    if (abs(coefficient) < epsilon) // Salta i termini ~ 0
        return;

    if (coefficient > 0) // Termini positivi preceduti da '+'
    {
        if (var.empty())
            equation << "+" << format_number(coefficient);

        else
        {
            if (abs(coefficient - 1.0) < epsilon) // Se il coefficiente ~ 1, +1x -> +x
                equation << "+" << var;

            else
                equation << "+" << format_number(coefficient) << var;
        }
    }

    else // Termini negativi preceduti da '-'
    {
        if (var.empty())
            equation << format_number(coefficient);

        else
        {
            if (abs(coefficient + 1.0) < epsilon) // Se il coefficiente ~ -1, -1x -> -x
                equation << "-" << var;

            else
                equation << format_number(coefficient) << var;
        }
    }
}

void print_equation(double a, double b, double c)
{
    const double epsilon = 1e-9;
    ostringstream equation;

    equation.setf(ios::fixed);
    equation.precision(3);
    equation << "x²+y²";

    // Composizione equazione con termini non nulli
    append_term(equation, a, "x", epsilon);
    append_term(equation, b, "y", epsilon);
    append_term(equation, c, "", epsilon); // Aggiunta termine noto

    string equation_str = equation.str();
    equation_str += " = 0";

    // Stampa risultato
    cout << GREEN << "Soluzione trovata!" << NC << endl;
    cout << equation_str << endl;
    press_a_key_to_continue();
}

void three_points()
{
    // Inserimento coordinate punti
    vector<Point> points(3);

    for (int i = 0; i < 3; i++)
    {
        cout << "Coordinate del punto " << i + 1 << " (x, y): ";
        cin >> points[i].x >> points[i].y;
    }

    cout << "Calcolo in corso..." << endl;

    // Controllo allineamento punti
    if (are_aligned(points[0], points[1], points[2]))
    {
        cerr << RED << "Errore: " << NC << "I 3 punti sono allineati" << endl;
        press_a_key_to_continue();
        return;
    }

    // Composizione matrice aumentata
    vector<vector<double>> matrix(3, vector<double>(4));

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
        int max_pivot_row = i;

        for (int j = i + 1; j < 3; j++)
            if (abs(matrix[j][i]) > abs(matrix[max_pivot_row][i]))
                max_pivot_row = j;

        if (max_pivot_row != i)
            swap(matrix[i], matrix[max_pivot_row]);

        // Controllo singolarità matrice
        const double epsilon = 1e-9;

        if (abs(matrix[i][i]) < epsilon)
        {
            cerr << RED << "Errore: " << NC "La matrice è singolare, impossibile calcolare una soluzione" << endl;
            press_a_key_to_continue();
            return;
        }

        // Eliminazione gaussiana
        for (int j = i + 1; j < 3; j++)
        {
            double mult = matrix[j][i] / matrix[i][i];

            for (int k = i; k < 4; k++)
                matrix[j][k] -= matrix[i][k] * mult;
        }
    }

    // Back substitution
    vector<double> solution_values(3);

    for (int i = 2; i >= 0; i--)
    {
        double known = matrix[i][3];

        for (int j = i + 1; j < 3; j++)
            known -= matrix[i][j] * solution_values[j];

        solution_values[i] = known / matrix[i][i];
    }

    // Visualizzazione equazione
    print_equation(solution_values[0], solution_values[1], solution_values[2]);
}

void center_point()
{
    // Inserimento coordinate punti
    Point center, point;

    cout << "Coordinate del centro (x, y): ";
    cin >> center.x >> center.y;
    cout << "Coordinate del punto P (x, y): ";
    cin >> point.x >> point.y;

    // Calcolo dei coefficienti
    double a = -2 * center.x;
    double b = -2 * center.y;
    double radius_square = pow((point.x - center.x), 2) + pow((point.y - center.y), 2);
    double c = pow(center.x, 2) + pow(center.y, 2) - radius_square;

    // Visualizzazione equazione
    print_equation(a, b, c);
}

void center_radius()
{
    // Inserimento coordinate centro e misura raggio
    Point center;
    double radius;

    cout << "Coordinate del centro (x, y): ";
    cin >> center.x >> center.y;
    cout << "Misura del raggio: ";
    cin >> radius;

    // Controllo raggio >= 0
    if (radius < 0)
    {
        cerr << RED << "Errore: " << NC << "Il raggio è minore di 0" << endl;
        press_a_key_to_continue();
        return;
    }

    // Calcolo dei coefficienti
    double a = -2 * center.x;
    double b = -2 * center.y;
    double c = pow(center.x, 2) + pow(center.y, 2) - pow(radius, 2);

    // Visualizzazione equazione
    print_equation(a, b, c);
}

void menu()
{
    int option;

    do
    {
        cout << "Menu" << endl;
        cout << "1. Circonferenza dati 3 punti" << endl;
        cout << "2. Circonferenza dato il centro e un punto" << endl;
        cout << "3. Circonferenza dato il centro e il raggio" << endl;
        cout << "4. Esci" << endl;
        cout << "Seleziona un'opzione: ";
        cin >> option;

        clear_buffer();
        clear_screen();

        switch (option)
        {
        case 1:
            three_points();
            break;

        case 2:
            center_point();
            break;

        case 3:
            center_radius();
            break;

        case 4:
            cout << "Uscita in corso" << flush;
            dots();
            break;

        default:
            cerr << RED << "Errore: " << NC "Opzione invalida" << endl;
            press_a_key_to_continue();
            break;
        }
    } while (option != 4);
}

int main(int argc, char *argv[])
{
    if (get_os() == "Other")
    {
        cerr << RED << "Errore: " << NC << "Sistema operativo non supportato" << endl;
        return 1;
    }

    menu();

    return 0;
}