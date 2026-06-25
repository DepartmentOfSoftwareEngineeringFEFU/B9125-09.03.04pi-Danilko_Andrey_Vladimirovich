#include <iostream>
#include <clocale>

long long factorial(int num)
{
    long long fact = 1;
    for (int i = 2; i <= num; i++) fact *= i;
    return fact;
}

int comb(int n, int r)
{
    if (r == 1) return n;
    if (n == 1) return 1;
    return comb(n, r - 1) + comb(n - 1, r);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    int n, r;

    do {
        std::cout << "Введите n, r (0 <= r < n <= 10): ";
        std::cin >> n >> r;
    } while ((n < 1) || (n > 10) || (r < 0) || (r >= n));

    long long ans = factorial(n + r - 1) / (factorial(r) * factorial(n - 1));

    std::cout << "Точное значение:           " << ans << '\n';
    std::cout << "Полученное через рекурсию: " << comb(n, r) << '\n';

    return 0;
}
