#include <iostream>
#include <clocale>
#include <cmath>

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    int N, M;

    do {
        std::cout << "Введите через пробел количество строк и столбцов матрицы (каждое число от 3 до 10): ";
        std::cin >> N >> M;
    } while ((N < 3) || (N > 10) || (M < 3) || (M > 10));

    int matrix[10][10];

    for (int i = 0; i < N; i++) {
        std::cout << "Введите через пробел элементы " << (i + 1) << " строки: ";
        for (int j = 0; j < M; j++) {
            std::cin >> matrix[i][j];
        }
    }

    int count = 0;

    std::cout << "Номера строк, которые не образуют геометрическую прогрессию: ";
    
    for (int i = 0; i < N; i++) {
    
        bool gp = true;
        
        for (int j = 1; j < M - 1; j++) {
            if (matrix[i][j] * matrix[i][j] != matrix[i][j - 1] * matrix[i][j + 1]) {
                gp = false;
                break;
            }
        }
        
        if (!gp) {
            if (count == 0) std::cout << (i + 1);
            else            std::cout << ", " << (i + 1);
            count++;
        }
    }
    
    if (count == 0) std::cout << "таких нет";

    std::cout << '\n';

    return 0;
}