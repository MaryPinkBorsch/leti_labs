// Добро пожаловать! Это работа Калюжной М.И., студентки из 3352 группы, задание №1, вариант № 2, начало работы 07.09.23

#include <iostream>

#include <iomanip>
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "Russian");
    float x, A, P, Q; const float a1 = -24.555, a2 = 3214.432, b1 = 876.542, b2 = 324.908, c1 = 84632.4326, c2 = 23.754;
    std::cout << "Добро пожаловать! Это работа Калюжной М.И., студентки из 3352 группы, задание №1, вариант № 2, начало работы 07.09.23 \n";
    std::cout << "Дано: a1 * x^6 - b1 * x^4 + c1 * x^3 \na1 = -24.555 , a2 = 3214.432, b1 = 876.542, b2 = 324.908, c1 = 84632.4326, c2 = 23.754  \n";
    std::cout << "Пожалуйста, введите х(-100; +100): ";
    std::cin >> x;

    
    P = (a1 * x * x * x * x * x * x) - (b1 * x * x * x * x) + (c1 * x * x * x);
    std::cout << "Шаг 1:" << std::setw(20) << std::setprecision(9) << P << "\n";

    

    std::cout << "Дано: a2 * x^10 + b2 * x^9 - c2  \n";

    Q = (a2 * x * x * x * x * x * x * x * x * x * x) + (b2 * x * x * x * x * x * x * x * x * x) - c2;
    std::cout << "Шаг 2:" << std::setw(20) << std::setprecision(9) << Q << "\n";
    

    A = P / Q;
    std::cout << "Для x:" << std::setw(20) << std::setprecision(9) << x << "\n";
    std::cout << "Результат деления многочленов:" << std::setw(20) << std::setprecision(9) << A << "\n";
}
 
