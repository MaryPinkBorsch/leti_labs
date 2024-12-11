#include "Chaining_HashMap.h"
#include "Open_adressingHM.h"
#include <unordered_map> // для тестов
#include <chrono>

using namespace std;

struct MyHash // стуктура для ХЭШИРОВАНИЯ инт (ставит в соответсвтвие каждому ключу число)
{
    size_t operator()(const int &key)
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
        Chaining_HashMap<int, int> chapa(1000);
        for (int i = 0; i < 100; ++i)
        {
            chapa.put(i, i);
        }
        chapa.remove(6);
        int res;
        chapa.get(6, res);
    }
    { // хеш мапа на открытой адресации
        OA_hash_map<int, int, MyHash> mapa(1000);
        for (int i = 0; i < 100; ++i)
        {
            mapa.insert(i, i);
        }

        int tmp = 0;
        bool found = mapa.find_and_get(50, tmp);
        if (!found)
            cout << "ОШИБКА" << endl;
        mapa.remove(50);
        found = mapa.find_and_get(50, tmp);
        if (found)
            cout << "ОШИБКА1" << endl;
        found = mapa.find_and_get(150, tmp);
        if (found)
            cout << "ОШИБКА2" << endl;
    }

    // тесты
    {
        unordered_map<int, int> main_map; // map boss
        Chaining_HashMap<int, int> map1(1000);
        OA_hash_map<int, int, MyHash> map2(1000);

        for (int i = 0; i < 500; i++)
        {
            main_map[i] = i;
            map1.put(i, i);
            map2.insert(i, i);
        }
        if (map2.cur_size != main_map.size() || map1.cur_size != main_map.size() || map2.cur_size != map1.cur_size)
            cout << " ОШИБКА size 1!" << endl;

        for (int i = 0; i < 500; i++)
        {
            int res = -1;
            map1.get(i, res);
            if (res != i)
                cout << " ОШИБКА 2!" << endl;
            if (map2.find_and_get(i, res) != true || res != i)
                cout << " ОШИБКА 23!" << endl;
        }
        for (int i = 0; i < 500; i += 2)
        {
            main_map.erase(i);
            map1.remove(i);
            map2.remove(i);
            for (int j = 0; j < 500; j++)
            {
                if (j <= i && j % 2 == 0)
                    continue;
                int map2_val = -1;
                bool found_in_map2 = map2.find_and_get(j, map2_val);
                if (!found_in_map2)
                    abort();
            }
        }
        for (int i = 0; i < 500; i++)
        {
            auto it = main_map.find(i);
            int main_val = -1;
            bool found_in_main_map = it != main_map.end();
            if (found_in_main_map)
                main_val = it->second;

            int map1_val = -1;
            bool found_in_map1 = map1.get(i, map1_val);
            int map2_val = -1;
            bool found_in_map2 = map2.find_and_get(i, map2_val);
            if (found_in_main_map != found_in_map1 || main_val != map1_val)
                cout << " ОШИБКА 24!" << endl;
            if (found_in_main_map != found_in_map2 || main_val != map2_val)
                cout << " ОШИБКА 25!" << endl;
        }
    }

    // тесты ЗАМЕРЫ ВРЕМЕНИ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        unordered_map<int, int> main_map; // map boss
        Chaining_HashMap<int, int> map1(10);
        OA_hash_map<int, int, MyHash> map2(10);

        int NUM_ELEM = 0;
        int gap = 1000;
        static const int MAX_NUM_ELEM = 50000;

        while (NUM_ELEM <= MAX_NUM_ELEM)
        {
            cout << endl
                 << endl
                 << "Кол-во элементов в мапе " << NUM_ELEM << endl
                 << endl;
            cout << "ВСТАВКА" << endl;

            // замеры на вставку
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    main_map[i] = i;
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "main_map " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    map1.put(i, i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map1 chaining " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    map2.insert(i, i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map2 OA " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            cout << endl
                 << "ПОИСК " << endl;
            // замеры на поиск + доступ к значению
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    main_map.find(i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "main_map " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            {
                int res = -1;
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    map1.get(i, res);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map1 chaining " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    map2.find_and_get(i, i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map2 OA " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            cout << endl
                 << "УДАЛЕНИЕ " << endl;
            // замеры на удаление значений
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    main_map.erase(i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "main_map " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            {
                int res = -1;
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    map1.remove(i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map1 chaining " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                for (int i = 0; i < NUM_ELEM; i++)
                {
                    map2.remove(i);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map2 OA " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0 << " ms" << std::endl;
            }

            NUM_ELEM += gap;
        }
    }
    // НОВЫЕ ЗАМЕРЫ
    {
        //  {
        //      std::cout << "Новые замеры: зависимость времени вставки 100 К элементов от load Factor от 0 до 0,9 с шагом 0,01" << std::endl;
        //      OA_hash_map<int, int, MyHash> map2(1000000); // макс размер 10000000, шаг 1000000 (0.01 лоад фактор) из которых замеряются 10000
        //      for (int i = 0; i < 90; ++i)
        //      {
        //          // замеряем вставку 100 К элементов на лоад факторе i / 10.0
        //          std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
        //          for (int j = 0; j < 10000; j++)
        //          {
        //              map2.insert(i * 100000 + j, i * 100000 + j);
        //          }
        //          std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        //          std::cout << "map2 OA вставка 100000 эл-в на " << (double)map2.cur_size / map2.max_size << " лоад факторе " << " " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000 << " milliseconds" << std::endl;
        //          // for (int j = 100000; j < 1000000; j++)
        //          // {
        //          //     map2.insert(i * 1000000 + j, i * 1000000 + j);
        //          // }
        //      }
        //  }
        // {
        //     std::cout << endl
        //               << "Новые замеры: зависимость времени поиска 100 К элементов от load Factor от 0 до 0,9 с шагом 0,01" << std::endl;
        //     OA_hash_map<int, int, MyHash> map2(1000000); // макс размер 10000000, шаг 1000000 (0.01 лоад фактор) из которых замеряются 10000
        //     for (int i = 0; i < 90; ++i)
        //     {
        //         for (int j = 0; j < 10000; j++)
        //         {
        //             map2.insert(i * 100000 + j, i * 100000 + j);
        //         }
        //         // замеряем поиск 100 К элементов на лоад факторе i / 10.0
        //         std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
        //         for (int j = 0; j < 10000; j++)
        //         {
        //             int val;
        //             map2.find_and_get(i * 100000 + j, val);
        //         }
        //         std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        //         std::cout << "map2 OA поиск 100000 эл-в на " << (double)map2.cur_size / map2.max_size << " лоад факторе " << " " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000 << " milliseconds" << std::endl;
        //     }
        // }
        {
            std::cout << endl
                      << "Новые замеры: зависимость времени удаления 100 К элементов от load Factor от 0 до 0,9 с шагом 0,01" << std::endl;
            OA_hash_map<int, int, MyHash> map2(10000000); // макс размер 10000000, шаг 1000000 (0.01 лоад фактор) из которых замеряются 10000
            for (int j = 0; j < 9000000; j++)
            {
                map2.insert(j*10, j*10);
            }
            for (int i = 90; i >= 0; --i)
            {
                std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
                for (int j = 0; j < 100000; j++)
                {
                    map2.remove(i * 1000000 + j*10);
                }
                std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                std::cout << "map2 OA удаление 100000 эл-в на " << (double)map2.cur_size / map2.max_size << " лоад факторе " << " " << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000 << " milliseconds" << std::endl;
            }
        }
    }

    return 0;
}