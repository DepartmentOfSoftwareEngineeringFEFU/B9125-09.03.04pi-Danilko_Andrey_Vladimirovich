#include <iostream>
#include <cmath>
#include <iomanip>
#include <clocale>

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    double eps;
    std::cout << "Введите точность eps (0 < eps < 1): ";
    std::cin >> eps;

    const double a = M_PI / 4;
    double s, d;

    int n = 0;
    double x;

    do {
        if (n % 2 == 0) {
            x = 1.0 / (2 * n + 1);
        }
        else {
            x = -1.0 / (2 * n + 1);
        }
        s += x;
        n++;
    } while (std::fabs(s - a) >= eps);

    d = std::fabs(s - a);

    std::cout << std::fixed << std::setprecision(10);
    
    std::cout << "Полученное приближение:        " << s << '\n';
    std::cout << "Приблизительное значение пи/4: " << a << "...\n";
    std::cout << "Разность:                      " << d << '\n';

    return 0;
}