// Добро пожаловать! Это работа Калюжной М.И., студентки из 3352 группы, задание №1, вариант № 1, начало работы 07.09.23

#include <iostream>

#include <iomanip>

int main(void)
{
    float x, P, Q, P1, P2, P3, Q1, Q2, Q3; const float a1 = -24.555 , a2 = 3214.432, b1 = 876.542, b2 = 324.908, c1 = 84632.4326, c2 = 23.754;

    std::cout << "Welcome! This is a Homework of Kalujnaya Maria , from 3352 groupe, Task 1, var. 1, begin 07.09.23 \n";
    std::cout << "Please, enter х(-100; +100): ";
    std::cin >> x;
    P1 = a1 * x*x*x;
    std::cout << "Step 1:" << std::setw(20) << std::setprecision(7) << P1 << "\n";
    P2 = P1 - b1 * x + c1;
    std::cout << "Step 2:" << std::setw(20) << std::setprecision(7) << P2 << "\n";
    P3 = x * x * x * P2;
    std::cout << "Step 3:" << std::setw(20) << std::setprecision(7) << P3 << "\n";
    P = P3;
    std::cout << "For х:" << std::setw(20) << std::setprecision(7) << x << "\n";
    std::cout << "Result:" << std::setw(20) << std::setprecision(7) << P << "\n";
 





}

