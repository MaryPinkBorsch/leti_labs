#include <bitset>
#include <iostream>

// надо оформить ввод с клавы чара и интервала для работы с
// и предусмотреть разных неадекватных пользователей (вывод предупреждейний)
// уточнить про то как именно делать сдвиги

// типы: unsigned char и double

// Меню:
// 1 = ввод unsigned char
// 2 = ввод double
// 3 = вывод в двоичном виде
// 4 = выполнение преобразования
// 5 = выход

enum DataType
{
    UnsignedCharType,
    DoubleType
};

struct Data // структура типа данных
{
    DataType type; // енум позволяющий узнать какой у нас тип - дабл или чар
    union          // мы можем хранить или значение дабла д или чара сч
    {
        unsigned char uc;
        double d;
    };
};

using namespace std;

// функции для выделения заданного интервала из числа (например биты с 6 по 20 из дабла, или со 2 по 5 из чара)

// для получения результата, нам надо сделать маску
// применить маску к заданному числу с помощью побитового "и"

// например: для ввода 10101010 нам
// надо получить 3 бита начиная с 3ьего бита, в таком случае
// маска должна быть 00111000, тогда после логисечкого И : 00101000

// чтобы получить маску надо:
//  1. сдвинуть 1 на длину маски влево
//  2. вычесть из результата 1 (все нули справа от 1 станут единицами)
//  3. сдвинуть результат влево на start (индекс первого бита в интервале)

unsigned long long extract_bits_char(char input, unsigned long long start /*начиная с какого бита*/, unsigned long long num /*сколько взять*/)
{
    // start - индексация от нуля ( от LSB к MSB (т.е. справа налево))
    // готовим маску

    unsigned long long mask = ((1 << num) - 1) << start;
    unsigned long long res = input;
    res = res & mask;
    return res;
}

unsigned long long extract_bits_double(double input, unsigned long long start /*начиная с какого бита*/, unsigned long long num /*сколько взять*/)
{
    // делаем из дабла тип лонг лонг в побитовом представлении
    unsigned long long res = (*(reinterpret_cast<unsigned long long *>(&input)));
    unsigned long long mask = ((1 << num) - 1) << start;
    res = res & mask;
    return res;
}

int count_bits(unsigned long long data)
{

    int num1 = 0; // количество 1
    for (int i = 0; i < 64; i++)
    {
        if (data & (1 << i)) // мы проверяем с помощью маленькой маски какой бит стоит на i-той позиции в числе
            num1++;
    }
    return num1;
}

void get_interval(Data &data, int &bit1, int &num)
{

    while (1)
    {
        cout << "Введите индекс первого бита в интервале (индексация идет от 0): " << endl;
        cin >> bit1;
        if (bit1 < 0)
        {
            cout << " Ошибка! Неправильный ввод значения! " << endl;
            continue;
        }
        if (data.type == DoubleType && bit1 > 63)
        {
            cout << " Ошибка! Неправильный ввод значения double! " << endl;
            continue;
        }
        if (data.type == UnsignedCharType && bit1 > 7)
        {
            cout << " Ошибка! Неправильный ввод значения unsigned char! " << endl;
            continue;
        }
        break;
    }
    while (1)
    {
        cout << "Введите кол-во битов в интервале: " << endl;
        cin >> num;
        if (num < 0)
        {
            cout << " Ошибка! Неправильный ввод значения! " << endl;
            continue;
        }
        if (data.type == DoubleType && bit1 + num > 63)
        {
            cout << " Ошибка! Неправильный ввод значения интервала double! " << endl;
            continue;
        }
        if (data.type == UnsignedCharType && bit1 + num > 7)
        {
            cout << " Ошибка! Неправильный ввод значения  интервала unsigned char! " << endl;
            continue;
        }
        break;
    }
}

void process(Data &data)
{
    int bit1 = -1; // первый бит в интревале
    int num = -1;  // длина интервала
    get_interval(data, bit1, num);
    // 1 = сдвиг влево

    switch (data.type)
    {
    case UnsignedCharType:
    {
        // двоичное представление интервала
        unsigned long long interval_bits = extract_bits_char(data.uc, bit1, num);

        // кол-во единиц
        int num1 = count_bits(interval_bits);

        // кол-во 0 в интервале
        int num0 = num - num1;
        int sdvig = abs(num1 - num0);
        if (sdvig == 0) // ничего не сдвигаем
        {
            cout << "Итоговый сдвиг = 0, число не изменилось" << endl;
            return;
        }

        unsigned char mask = ((1 << num) - 1) << bit1;
        unsigned char inverted_mask = ~mask; // инвертируем маску

        // на примере:
        // 10101010 - исходное число
        // 00111000 - маска* для 3, 3
        // 00101000 - interval_bits
        // 00010100 - результат сдвига interval_bits при sdvig = 1 вправо    (0)
        // 11000111 - нужна инвертированная маска!
        // 10000010 - результат логического И исходного числа и инвертированной маски   (1)
        // 00001000 - результат логического И реузльтата сдвига (0) и маски*     (2)
        // 10001010 - конечный результат логического ИЛИ между (1) и (2)

        if (num1 > num0) // делаем сдвиг вправо
        {
            interval_bits = interval_bits >> sdvig; // (0)
        }
        else // сдвиг влево
        {
            interval_bits = interval_bits << sdvig; // (0)
        }

        unsigned char tmp1 = data.uc & inverted_mask; // (1)
        unsigned char tmp2 = interval_bits & mask;    // (2)
        data.uc = tmp1 | tmp2;                        // (конечный результат)
    }
    break;
    case DoubleType:
    {
        unsigned long long chislo = (*(reinterpret_cast<unsigned long long *>(&data.d)));
        // двоичное представление интервала
        unsigned long long interval_bits = extract_bits_double(data.d, bit1, num);

        // кол-во единиц
        int num1 = count_bits(interval_bits);

        // кол-во 0 в интервале
        int num0 = num - num1;
        int sdvig = abs(num1 - num0);
        if (sdvig == 0) // ничего не сдвигаем
        {
            cout << "Итоговый сдвиг = 0, число не изменилось" << endl;
            return;
        }

        unsigned long long mask = ((1 << num) - 1) << bit1;
        unsigned long long inverted_mask = ~mask; // инвертируем маску

        if (num1 > num0) // делаем сдвиг вправо
        {
            interval_bits = interval_bits >> sdvig; // (0)
        }
        else // сдвиг влево
        {
            interval_bits = interval_bits << sdvig; // (0)
        }

        unsigned long long tmp1 = chislo & inverted_mask; // (1)
        unsigned long long tmp2 = interval_bits & mask;   // (2)
        chislo = tmp1 | tmp2;                             // (конечный результат)
        data.d = (*(reinterpret_cast<double *>(&chislo)));
    }
    break;
    default:
    {
    }
    break;
    }
}

void print_main_menu()
{

    cout << "Меню : " << endl
         << " 1 = ввод unsigned char " << endl
         << " 2 = ввод double " << endl
         << " 3 = вывод в двоичном виде " << endl
         << " 4 = выполнение преобразования " << endl
         << " 5 = выход " << endl;
}

void print_data(Data &data)
{
    switch (data.type)
    {
    case UnsignedCharType:
    {
        std::cout << std::bitset<8>(data.uc) << std::endl; // в чаре 8 битов (1 байт)
    }
    break;
    case DoubleType:
    {
        // так как битсет не умеет создаваться из дабла то
        // мы его заставим (реинтерпрет каст указателя на дабл к указателю на лонг лонг и разименование)
        std::cout << std::bitset<64>(*(reinterpret_cast<unsigned long long *>(&data.d))) << std::endl; // в дабле 8 байтов
    }
    break;
    default:
    {
    }
    break;
    }
}

int main(int argc, char *argv[])
{
    cout << "Добро пожаловать в лаб М.Калюжной. Е нижейко и Ю. Парфеновой " << endl
         << " Bведите число от 1 до 5 в меню." << endl;
    char choice = -1;
    while (choice != '5')
    {
        Data tmp;
        print_main_menu();
        cin >> choice;
        switch (choice)
        {
        case '1':
        {
            tmp.type = UnsignedCharType;
            int value;
            cin >> value; // вводится инт а после уже преобразуется в чар (чтоб было то число какое мы ввели)
            // тут нужна проверка на подходячесть
            while (value < 0)
            {
                cout << "Значение введено некорректно, повторите ввод" << endl;
                cin >> value;
            }
            tmp.uc = value;
        }
        break;
        case '2':
        {
            tmp.type = DoubleType;
            cin >> tmp.d; // нужна ли тут проверка? хз
        }
        break;
        case '3':
        {
            print_data(tmp);
        }
        break;
        case '4':
        {

            process(tmp);
        }
        break;
        default:
            break;
        }
    }
    return 0;
}