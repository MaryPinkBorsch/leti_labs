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
    switch(data.type) 
    {
        case UnsignedCharType:
        {
            std::cout<<std::bitset<8>(data.uc)<<std::endl; // в чаре 8 битов (1 байт)
        }
        break;
        case DoubleType:
        {
            // так как битсет не умеет создаваться из дабла то
            // мы его заставим (реинтерпрет каст указателя на дабл к указателю на лонг лонг и разименование)
            std::cout<<std::bitset<64>(*(reinterpret_cast<unsigned long long*>(&data.d)))<<std::endl; // в дабле 8 байтов
        }
        break;
        default:
        {
        }
        break;
    }
}

void process()
{
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
            while(value < 0)
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
            process();
        }
        break;
        default:
            break;
        }
    }
    return 0;
}