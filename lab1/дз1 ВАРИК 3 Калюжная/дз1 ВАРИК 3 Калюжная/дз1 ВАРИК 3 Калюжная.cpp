// Добро пожаловать! Это работа Калюжной М.И., студентки из 3352 группы, задание №1, вариант № 3, начало работы 07.09.23

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <locale.h>

int main(void)
{
    setlocale(LC_ALL, "Russian");
    float x, B, A, P, Q, P1, P2, P3, Q1, Q2, Q3; const float a1 = -24.555, a2 = 3214.432, b1 = 876.542, b2 = 324.908, c1 = 84632.4326, c2 = 23.754;

    printf(" % s", "Добро пожаловать! Это работа Калюжной М.И., студентки из 3352 группы, задание №1, вариант № 3, начало работы 07.09.23 \n");
    printf(" % s", "Дано: a1 * x^6 - b1 * x^4 + c1 * x^3 \na1 = -24.555 , a2 = 3214.432, b1 = 876.542, b2 = 324.908, c1 = 84632.4326, c2 = 23.754  \n");
    printf(" % s", "Пожалуйста, введите х(-100; +100):");
    /*scanf("% f", & x );*/
    scanf_s("%f", &x);
    P1 = a1 * x * x * x;
    printf("% s, %f \n", " Шаг 1:", P1);
    P2 = P1 - (b1 * x) + c1;
    printf("% s, %f \n", " Шаг 2:", P2);
    P3 = x * x * x * P2;
    printf("% s, %f \n", " Шаг 3:", P3);



   /* 
    std::cout << "Пожалуйста, введите х(-100; +100): ";
    std::cin >> x;
    P1 = a1 * x * x * x;
    std::cout << "Шаг 1:" << std::setw(20) << std::setprecision(9) << P1 << "\n";
    P2 = P1 - b1 * x + c1;
    std::cout << "Шаг 2:" << std::setw(20) << std::setprecision(9) << P2 << "\n";
    P3 = x * x * x * P2;
    std::cout << "Шаг 3:" << std::setw(20) << std::setprecision(9) << P3 << "\n";
    P = P3;
    std::cout << "Для x:" << std::setw(20) << std::setprecision(9) << x << "\n";
    std::cout << "Результат первого многочлена:" << std::setw(20) << std::setprecision(9) << P << "\n";

    std::cout << "Дано: a2 * x^10 + b2 * x^9 - c2  \n";
    Q1 = a2 * x + b2;
    std::cout << "Шаг 4:" << std::setw(20) << std::setprecision(9) << Q1 << "\n";
    Q2 = x * x * x * x * x * x * x * x * x * Q1;
    std::cout << "Шаг 5:" << std::setw(20) << std::setprecision(9) << Q2 << "\n";
    Q3 = Q2 - c2;
    std::cout << "Шаг 6:" << std::setw(20) << std::setprecision(9) << Q3 << "\n";
    Q = Q3;
    std::cout << "Для x:" << std::setw(20) << std::setprecision(9) << x << "\n";
    std::cout << "Результат второго многочлена:" << std::setw(20) << std::setprecision(9) << Q << "\n";

    A = P / Q;
    std::cout << "Для x:" << std::setw(20) << std::setprecision(9) << x << "\n";
    std::cout << "Результат деления многочленов:" << std::setw(20) << std::setprecision(9) << A << "\n";*/





}

