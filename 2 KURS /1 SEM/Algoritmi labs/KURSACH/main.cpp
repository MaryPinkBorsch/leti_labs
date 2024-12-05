#include "Chaining_HashMap.h"
#include "Open_adressingHM.h"

using namespace std;

struct MyHash // стуктура для ХЭШИРОВАНИЯ инт (ставит в соответсвтвие каждому ключу число)
{
    size_t operator()(const int & key) 
    {
        static const int prime1 = 31;
        static const int prime2 = 65521;
        // это простые числа
        return (key * prime1 + prime2);
    }
};

int main(int argc, char *argv[])
{
    { // хеш мапа на списках
        Chaining_HashMap chapa;
        for (int i = 0; i < 100; ++i)
        {
            chapa.put(i, i);
        }
    }
    { // хеш мапа на открытой адресации
        OA_hash_map<int, int, MyHash> mapa;
        for (int i = 0; i < 100; ++i)
        {
            mapa.insert(i, i);
        }

        int tmp = 0;
        bool found = mapa.find_and_get(50, tmp);
        if (!found)
            cout << "ОШИБКА" << endl;
        found = mapa.find_and_get(150, tmp);
        if (found)
            cout << "ОШИБКА2" << endl;
    }

    return 0;
}