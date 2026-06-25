#include <iostream>
#include <clocale>

long double scalar(const double* p, const double* q, std::size_t size)
{
    double res = 0.0;

    for (std::size_t i = 0; i < size; ++i) {
        res += p[i] * q[i];
    }

    return res;
}


double min(double* arr, std::size_t size)
{
    double res = arr[0];

    for (std::size_t i = 1; i < size; i++) {
        if (arr[i] < res) res = arr[i];
    }

    return res;
}

double max(double* arr, std::size_t size)
{
    double res = arr[0];

    for (std::size_t i = 1; i < size; i++) {
        if (arr[i] > res) res = arr[i];
    }

    return res;
}


int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::string vecs = "xyz";

    const int size = 2;

    double x[size], y[size], z[size];
    
    for (int i = 0; i < 3; i++) {
        std::cout << "Введите через пробел элементы вектора " << vecs[i] << ": ";
        for (int j = 0; j < size; j++) {
            switch (i) {
                case 0: std::cin >> x[j]; break;
                case 1: std::cin >> y[j]; break;
                case 2: std::cin >> z[j]; break;
            }
        }
    }
    
    double mins[3] = { min(x, size), min(y, size), min(z, size) };
    
    double max_min = mins[0];
    int min_i = 0;

    for (int i = 1; i < 3; i++) {
        if (mins[i] > max_min) {
            max_min = mins[i];
            min_i = i;
        }
    }

    double a[size], b[size], c[size];

    for (int i = 0; i < size; i++) {
        switch (min_i) {
            case 0: a[i] = x[i], b[i] = y[i]; c[i] = z[i]; break;
            case 1: a[i] = y[i], b[i] = x[i]; c[i] = z[i]; break;
            case 2: a[i] = z[i], b[i] = x[i]; c[i] = y[i]; break;
        }
    }

    long double r = scalar(a, a, size) - scalar(b, c, size);

    std::cout << "Скалярное произведение: " << r << '\n';

    return 0;
}