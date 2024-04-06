#include <iostream>
#include <math.h>
#include <locale.h>
#include <iomanip>
#include "fstream"
using namespace std;

int main(int argc, char *argv[])
{
    ofstream out;
    setlocale(LC_ALL, "Russian");
    float x, eps, q, Un, Sn;
    int k, n, z;
    k = 0;
    n = 1;
    std::cout << "Приветствую! Это лабораторная работа №3 Калюжной М.И. группа 3352 Вариант 61  начало работы 04.10.23, конец 10.10." << std::endl;
    std::cout << "Дана формула бесконечной суммы для математ. ряда начиная с n = 1: " << std::endl;
    std::cout << "(-1)^n * x^n / 5^n" << std::endl;
    std::cout << "Введите пж х (-1<x<1)" << std::endl;
    std::cin >> x;
    std::cout << "Введите пж eps (abs(eps) < 10^(-10) и eps > 0)" << std::endl;
    std::cin >> eps;
    while (k < 3)
    {
        if (std::abs(eps) > pow(10, -10) || eps < 0)
        {
            std::cout << "Введенные данные не удовлетворяют начальным условиям" << std::endl;
            std::cout << "Введите пж eps (abs(eps) < 10^(-10)  и eps > 0)" << std::endl;
            std::cin >> eps;
            k = k + 1;
        }
        else
            break;
    }

    while (z < 3)
    {
        if (std::abs(x) > 1)
        {
            std::cout << "Введенные данные не удовлетворяют начальным условиям" << std::endl;
            std::cout << "Введите пж пж х (-1<x<1)" << std::endl;
            std::cin >> x;
            z = z + 1;
        }
        else
            break;
    }

    if (k > 3 || z > 3)
    {
        std::cout << "Введенные данные не удовлетворяют начальным условиям!!! Количество попыток исчерпано." << std::endl;
        return 1;
    }

    q = -1 * x / 5;
    Un = q;
    Sn = Un;

    std::cout << "Введите имя файла для сохранения" << std::endl;
    std::string fileName;
    std::cin >> fileName;
    out.open(fileName);
    //    out.open("D:/C++/lab3/outfile.txt");

    cout << "\n";
    cout << "---------------------------------------------\n";
    cout << "| n    | Un            | Sn                 |\n";
    cout << "---------------------------------------------\n";
    cout << "| " << left << scientific << setw(4) << n << " | " << setw(13) << setprecision(5) << Un << " | " << setw(18) << setprecision(11) << Sn << " | " << endl;

    out << "---------------------------------------------\n";
    out << "| n    | Un            | Sn                 |\n";
    out << "---------------------------------------------\n";
    out << "| " << left << scientific << setw(4) << n << " | " << setw(13) << setprecision(5) << Un << " | " << setw(18) << setprecision(11) << Sn << " | " << endl;

    while ((fabs(Un) >= eps) && (n < 1000))
    {
        Un *= q;
        Sn += Un;
        n += 1;

        cout << "| " << left << scientific << setw(4) << n << " | " << setw(13) << setprecision(5) << Un << " | " << setw(18) << setprecision(11) << Sn << " | " << endl;
        out << "| " << left << scientific << setw(4) << n << " | " << setw(13) << setprecision(5) << Un << " | " << setw(18) << setprecision(11) << Sn << " | " << endl;
    }
    cout << "---------------------------------------------\n";
    cout << "\n\nРезультат: ";
    cout << "n = " << n;
    cout << "\n";
    cout << "| " << left << scientific << setw(4) << n << " | " << setw(13) << setprecision(5) << Un << " | " << setw(18) << setprecision(11) << Sn << " | " << endl;

    out << "---------------------------------------------\n";
    out << "\n\nРезультат: ";
    out << "n = " << n;
    out << "\n";
    out << "| " << left << scientific << setw(4) << n << " | " << setw(13) << setprecision(5) << Un << " | " << setw(18) << setprecision(11) << Sn << " | " << endl;

    out.close();
}