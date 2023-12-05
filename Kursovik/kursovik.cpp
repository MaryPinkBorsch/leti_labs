#include <iostream>
#include <fstream>

#include <iomanip>
#include <math.h>

static const int MAX_DOTS_NUM = 1000;
static const int MAX_KAVADRATS_NUM = 1000;

// разобьем задачу на более простые
// сначала найдем все наборы точек которые составляют квадраты
// чтобы найти квадрат надо
// 1. посчитать все парные расстояния между точками (построить матрицу расстояний между двумя любыми точками)
// 2. для каждой точки отсортировать все остальные точки по расстояниям
// 3. обработать отсортированную последовательность - найти все пары точек для конкретной точки отстоящие от нее (этой точки) на одинаковое расстояние A
// 4. для каждой из таких пар попытаться найти еще одну на расстоянии A*Sqrt(2)
// 5. сравнить расстояния от диагональной точки до двух остальных точек  и от этих двух остальных до главной точки, они должны быть равны

//  см картинку 4:  мы сравниваем расстояние от точки 3 до т 1 с расстоянием от точки 1 то точки 0 и одновременнно с этим сравниваем расстояния 3-2 и 2-0, если они все равны, то это квадрат
// см рис 4: если 3-1 = 1-0 и 3-2 = 2-0, то это квадрат (все расстояния равны, 3-1 это расстояние от 3 до 1)
                        

//6. потом перебором найдем все квадраты которые имеют одинаковый центр и повернуты на 45 градусов
// из них выберем те у которых соотношение сторон подходящее ( КВАДРАТЫ ПЕРЕСЕКАЮТСЯ), это даст нам список всех подходящих фигур
// потом для каждой из фигур посчитать количество точек которые входят в нее (см рисунок, квадрат + 4 треугольничка)
// отсортировать и выбрать те куда попадает максимальное количество точек

using namespace std;
// объявим тип (структуру) "Точка", этот тип имеет 2 флоат переменные - х и у, коор-ты точки
// и метод для вычисления расстояния от другой точки (Евклидово расстояние)

struct dot
{
    // этот метод можно будет использговать так
    // dot A, B;
    // A.x = 1.0;
    // A.y = 1.0;
    // B.x = 2.0;
    // b.y = 2.0;
    // float distance = A.distance_from(B);
    float distance_from(const dot &other)
    {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    float x;
    float y;
};

struct kvadrat_index
{
    int idx; // индекс данного квадрата в массиве квадратов
    dot zentr; // точка - центр квадрата
};

bool read_dots(std::string filename, dot *&p_dots, int &num_dots)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (input.eof())
    {
        std::cout << " Файл input пуст, упс" << endl;
        return false;
    }
    num_dots = 0;
    while (!input.eof())
    {
        dot tmp;
        bool dot_x_empty = true;
        bool dot_y_empty = true;
        char next_char;
        next_char = input.peek();
        if (next_char != ' ' && next_char != '\r' && next_char != '\n')
        {
            input >> skipws >> tmp.x;
            dot_x_empty = false;
            while (input.peek() == ' ')
                input >> noskipws >> next_char;
            next_char = input.peek();

            if (next_char != ' ' && next_char != '\r' && next_char != '\n' && dot_x_empty == false)
            {
                input >> skipws >> tmp.y;
                dot_y_empty = false;
                if (dot_x_empty == false && dot_y_empty == false)
                {
                    p_dots[num_dots] = tmp;
                    num_dots++;
                }
            }
        }
        while (input.peek() == ' ' || input.peek() == '\r' || input.peek() == '\n')
            input >> noskipws >> next_char;
    }
    return true;
}

void calculate_pair_distance(dot *&dots, int &num_dots, float **&distances, int &num_distances)
{

    distances = new float *[num_distances];

    for (int i = 0; i < num_distances; i++)
    {
        distances[i] = new float[num_distances];
    }

    for (int i = 0; i < num_distances; i++)
    {
        distances[i][i] = 0;
        for (int j = i + 1; j < num_distances; j++)
        {
            distances[i][j] = dots[i].distance_from(dots[j]);
            distances[j][i] = distances[i][j];
        }
    }
}

struct distance_index
{
    int idx;        // индекс другооой точки
    float distance; // расстояние до этой другой точки
};

void sort_pair_distances(dot *&dots, int &num_dots, float **&distances, int &num_distances, distance_index **&distance_indexes, int &num_indicies)
{
    // мы получили пустой массив его надо заполнить и отсортировать

    distance_indexes = new distance_index *[num_indicies]; // память для внешнего измерения указателей

    for (int i = 0; i < num_indicies; i++)
    {
        distance_indexes[i] = new distance_index[num_indicies];
    }

    // заполнение массивов расстояния до всех точек для каждой точки
    for (int i = 0; i < num_distances; i++)
    {
        for (int j = i; j < num_distances; j++)
        {
            distance_index tmp;
            tmp.idx = j;
            tmp.distance = distances[i][j];
            distance_indexes[i][j] = tmp;
            if (i != j)
            {
                tmp.idx = i;
                distance_indexes[j][i] = tmp;
            }
        }
    }

    // sorting

    // проход по всем точкам
    for (int i = 0; i < num_distances; i++)
    {
        // для каждой точки надо отсортировать расстояния до других точек
        bool sorted = false;
        while (sorted == false) //! sorted
        {
            sorted = true; // надеемся на то что оно уже отсортированно
            for (int j = 0; j < num_distances - 1; j++)
            {
                if (distance_indexes[i][j].distance > distance_indexes[i][j + 1].distance)
                {
                    distance_index tmp = distance_indexes[i][j];
                    distance_indexes[i][j] = distance_indexes[i][j + 1];
                    distance_indexes[i][j + 1] = tmp;
                    sorted = false;
                }
            }
        }
    }
}

bool binary_search(distance_index *&distance_indexes, int L, int R, float value, int &result_index)
{
    while (L <= R)
    {
        int mid = (L + R) / 2;
        if (distance_indexes[mid].distance == value) // что то там условие с эпсилон
        {
            result_index = mid;
            return true;
        }
        else if (distance_indexes[mid].distance < value)
        {
            L = mid + 1;
        }
        else if (distance_indexes[mid].distance > value)
        {
            R = mid - 1;
        }
    }
    return false;
}

void search_one_kvadrat(dot *&dots, int &num_dots, distance_index **&distance_indexes, int &num_indicies, int **&kvadrati, int &num_kvadrats)
{
    for (int i = 0; i < num_dots; i++)
    {
        for (int j = 0; j < num_dots; j++)
        {
            if (distance_indexes[i][j].distance == distance_indexes[i][j + 1].distance) // ПРОВЕРКА НА КАКОЕ-ТО МАЛОЕ ЧИСЛО ЭПСИЛОН ЧТО ЭТИ 2 ЧИСЛА РАВНЫ С ТОЧНОСТЬЮ ДО ЭПСИЛОН
            {
                int result_index = -1;
                if (binary_search(distance_indexes[i], j + 2, num_dots - 1, distance_indexes[i][j].distance * sqrt(2.0), result_index))
                {
                    if (dots[distance_indexes[i][result_index].idx].distance_from(dots[distance_indexes[i][j].idx]) == distance_indexes[i][j].distance &&
                     dots[distance_indexes[i][result_index].idx].distance_from(dots[distance_indexes[i][j + 1].idx]) == distance_indexes[i][j + 1].distance) // жуткая функция для определения квадрата
                    {
                        int new_kvadrat[4]; // тут хранятся индексы точек составляющие один квадрат
                        new_kvadrat[0] = i;
                        new_kvadrat[1] = distance_indexes[i][j + 1].idx;
                        new_kvadrat[2] = distance_indexes[i][result_index].idx;
                        new_kvadrat[3] = distance_indexes[i][j].idx;

                        bool sorted = false;
                        while (sorted == false) //! sorted
                        {
                            sorted = true; // надеемся на то что оно уже отсортированно
                            for (int j = 0; j < 3; j++)
                            {
                                if (new_kvadrat[j] > new_kvadrat[j + 1])
                                {
                                    int tmp = new_kvadrat[j];
                                    new_kvadrat[j] = new_kvadrat[j + 1];
                                    new_kvadrat[j + 1] = tmp;
                                    sorted = false;
                                }
                            }
                        }
                        // мы отсортировали индексы точек 1го квадрата и щас запихнем их в массив со всеми квадратами
                        // и щас запихнем их в массив со всеми квадратами онли если там такого уже нет
                        bool found = false;

                        for (int i = 0; i < num_kvadrats; i++)
                        {
                            // тут хранятся индексы точек составляющие один квадрат
                            if (kvadrati[i][0] == new_kvadrat[0] &&
                                kvadrati[i][1] == new_kvadrat[1] &&
                                kvadrati[i][2] == new_kvadrat[2] &&
                                kvadrati[i][3] == new_kvadrat[3])
                            {
                                found = true;
                                break;
                            }
                        }

                        if (found == false)
                        {
                            kvadrati[num_kvadrats] = new int[4];

                            kvadrati[num_kvadrats][0] = new_kvadrat[0];
                            kvadrati[num_kvadrats][1] = new_kvadrat[1];
                            kvadrati[num_kvadrats][2] = new_kvadrat[2];
                            kvadrati[num_kvadrats][3] = new_kvadrat[3];

                            num_kvadrats++;

                            cout << setw(10) << " ОДИН Квадрат составляют точки с индексами: " << new_kvadrat[0] << " " << new_kvadrat[1] << " " << new_kvadrat[2] << " " << new_kvadrat[3] << endl;
                            // см картинку 4:  мы сравниваем расстояние от точки 3 до т 1 с расстоянием от точки 1 то точки 0 и одновременнно с этим сравниваем расстояния 3-2 и 2-0, если они все равны, то это квадрат
                            // см рис 4: если 3-1 = 1-0 и 3-2 = 2-0, то это квадрат (все расстояния равны, 3-1 это расстояние от 3 до 1)
                        }
                    }
                }
            }
        }
    }
}

void figura_check(kvadrat_index & kvadrate1, kvadrat_index & kvadrate2, int kvadrat_idx1, int kvadrat_idx2, dot * & dots, int ** & kvadrati)
{
    // см. картинку 6, там нам надо найти все углы ... 
    // нужно найти коор-ты недостающих 8 точек (точек пересечений квадратов) и проверить их наличие в массиве dots
    //

    
    int kvadrat_dot_idx[8]; // массив на 8 точек с 2х квадратов, тут хранятся их индексы
    float ugli[7]; // массив углов меж точками
    //индексы точек сост-х 1й квадрат
    kvadrat_dot_idx[0] = kvadrati[kvadrat_idx1][0];
    kvadrat_dot_idx[1] = kvadrati[kvadrat_idx1][1];
    kvadrat_dot_idx[2] = kvadrati[kvadrat_idx1][2];
    kvadrat_dot_idx[3] = kvadrati[kvadrat_idx1][3];
    //индексы точек сост-х 2й квадрат
    kvadrat_dot_idx[4] = kvadrati[kvadrat_idx2][0];
    kvadrat_dot_idx[5] = kvadrati[kvadrat_idx2][1];
    kvadrat_dot_idx[6] = kvadrati[kvadrat_idx2][2];
    kvadrat_dot_idx[7] = kvadrati[kvadrat_idx2][3];

    // посчитаем углы меж первой dot1 и остальными точками по теореме косинусов
    dot dot1 = dots[kvadrat_dot_idx[0]];
    dot center = kvadrate1.zentr;
    for (int i = 1; i < 8; i++)
    {
        // см рис. 7
        dot dot2 = dots[kvadrat_dot_idx[i]]; // dot2 = dot_i
        int a = dot1.distance_from(center);
        int b = center.distance_from(dot2);
        int c = dot1.distance_from(dot2);

        float ugol = acos((a*a + b*b - c*c)/(2*a*b));
        ugli[i-1] = ugol;
    }
    // надо отсортировать углы ?


}

int main(int argc, char *argv[])
{
    std::cout << "Добро пожаловать в курсовик Калюжной Марии 3352 26.11.23 !" << endl
              << endl;

    string filename = "input.txt";
    dot *dots = nullptr; // указатель под динамический массив для хранения точек (и их координат)
    dots = new dot[MAX_DOTS_NUM]; 
    int num_dots = 0;

    float **distances = nullptr; // указатель на указатель - для динамического двумерного массива попарных расстояний

    distance_index **sorted_distance_indexes; // массивы 2мерный из структур содерж. расстояния от данной точки до других точек и их индексы

    // массив с индексами точек которые составляют квадраты
    int **kvadrati = nullptr;
    kvadrati = new int *[MAX_KAVADRATS_NUM];
    int num_kvadrats = 0;

    kvadrat_index *kvadrat_indexes = nullptr;
    kvadrat_indexes = new kvadrat_index[num_kvadrats];

    // загрузить координаты точек из файла

    if (read_dots(filename, dots, num_dots))
    {
        cout << setw(10) << "Координата X"
             << "  " << setw(10) << "Координата Y" << endl;
        int len = num_dots;
        for (int i = 0; i < len; i++)
        {
            cout << setw(10) << dots[i].x << "  " << setw(10) << dots[i].y << endl;
        }

        // 1. посчитать все парные расстояния между точками (построить матрицу расстояний между двумя любыми точками)
        calculate_pair_distance(dots, num_dots, distances, num_dots);

        int num_distances = num_dots;
        for (int i = 0; i < num_distances; i++)
        {
            for (int j = i + 1; j < num_distances; j++)
            {
                cout << "Расстояние между точкой номер " << i << " и " << j << " равно: " << setw(10) << distances[i][j] << endl;
            }
        }

        cout << endl
             << endl;
        // 2.  для каждой точки отсортировать все остальные точки по расстояниям

        sort_pair_distances(dots, num_dots, distances, num_dots, sorted_distance_indexes, num_dots);
        cout << "       Результаты сортировки: " << endl;
        for (int i = 0; i < num_dots; i++)
        {
            for (int j = 0; j < num_dots; j++)
            {
                cout << "Расстояние между точкой номер " << i << " и " << sorted_distance_indexes[i][j].idx << " равно: " << setw(10) << sorted_distance_indexes[i][j].distance << endl;
            }
        }
        cout << endl
             << endl;

        // 3. обработать отсортированную последовательность - найти все пары точек для конкретной точки отстоящие от нее (этой точки) на одинаковое расстояние A              
        // 4. для каждой из таких пар попытаться найти еще одну на расстоянии A*Sqrt(2)
        search_one_kvadrat(dots, num_dots, sorted_distance_indexes, num_dots, kvadrati, num_kvadrats);

        //5. заполним массив с индексами всех квадратов

        for (int i = 0; i < num_kvadrats; i++)
        {
            kvadrat_indexes[i].idx = i; // вычисляем центры квадратов через среднее арифметическое
            kvadrat_indexes[i].zentr.x = ( dots[kvadrati[i][0]].x + dots[kvadrati[i][1]].x + dots[kvadrati[i][2]].x + dots[kvadrati[i][3]].x)/ 4.0;
            kvadrat_indexes[i].zentr.y = ( dots[kvadrati[i][0]].y + dots[kvadrati[i][1]].y + dots[kvadrati[i][2]].y + dots[kvadrati[i][3]].y)/ 4.0;
        }

        for( int i = 0; i < num_kvadrats; i++)
        {
            for (int j = i+1; j < num_kvadrats; j++)
            {
                if (kvadrat_indexes[i].zentr.x == kvadrat_indexes[j].zentr.x &&
                 kvadrat_indexes[i].zentr.y == kvadrat_indexes[j].zentr.y)
                {
                    figura_check(kvadrat_indexes[i], kvadrat_indexes[j], i, j, dots, kvadrati);
                }
            }
        }




        // нужен лог файл
        // нужен файл с результатом
    }

    //////////////////////////////////////////////////DELeTING
    for (int i = 0; i < num_dots; i++)
    {
        delete[] distances[i];
    }
    delete[] distances;

    for (int i = 0; i < num_dots; i++)
    {
        delete[] sorted_distance_indexes[i];
    }
    delete[] sorted_distance_indexes;

    for (int i = 0; i < num_kvadrats; i++)
    {
        delete[] kvadrati[i];
    }
    delete[] kvadrati;

    delete[] kvadrat_indexes;

    delete[] dots;
    return 0;
}