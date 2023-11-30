#include <iostream>
#include <fstream>

#include <iomanip>
#include <math.h>

static const int MAX_DOTS_NUM = 1000;

// разобьем задачу на более простые
// сначала найдем все наборы точек которые составляют квадраты
// чтобы найти квадрат надо
// 1. посчитать все парные расстояния между точками (построить матрицу расстояний между двумя любыми точками)
// 2. для каждой точки отсортировать все остальные точки по расстояниям
// 3. обработать отсортированную последовательность - найти все пары точек отстоящие от нее на одинаковое расстояние A
// 4. для каждой из таких попытаться найти еще одну на расстоянии A*Sqrt(2)
// 5. для всех наборов из 4 точек подходящих под правила выше - проверить что углы между соседними точками 90гр.

// потом перебором найдем все квадраты которые имеют одинаковый центр и повернуты на 45 градусов
// из них выберем те у которых соотношение сторон подходящее, это даст нам список всех подходящих фигур
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

bool read_dots(std::string filename, dot * & p_dots, int & num_dots)
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

void calculate_pair_distance(dot * & dots, int & num_dots, float ** & distances, int & num_distances)
{
   // distances.resize(dots.size());
    for (int i = 0; i < sizeof(dots); i++)
    {
       // distances[i].resize(dots.size());
        distances[i][i] = 0; //!!!!!!!!!!
        for (int j = i + 1; j < sizeof(dots); j++)
        {
            distances[i][j] = dots[i].distance_from(dots[j]); //!!!!!!!!!!!!!!!!!!
           // distances[j][i] = distances[i][j];                //////
        }
    }
}

struct distance_index
{
    int idx;        // индекс др точки
    float distance; // расстояние до этой другой точки
};

void sort_pair_distances(dot * & dots, int & num_dots, float ** & distances, int & num_distances, distance_index * & distance_indexes, int & num_indicies)
{
    // мы получили пустой массив его надо заполнить и отсортировать
    // заполнение массивов расстояния до всех точек для каждой точки
    for (int i = 0; i < sizeof(dots); i++)
    {
        for (int j = i + 1; j < sizeof(dots); j++)
        {
            distance_index tmp;
            tmp.idx = j;
            tmp.distance = distances[i][j]; // ТУТ НУЖНА * или всетаки нет???? 
            distance_indexes[i]= tmp;
        }
    }
    // sorting
}

int main(int argc, char *argv[])
{
    std::cout << "Добро пожаловать в курсовик Калюжной Марии 3352 26.11.23 !" << endl
              << endl;
  
    string filename = "input.txt";
    dot * dots = nullptr; // указатель под динамический массив для хранения точек (и их координат)
    dots = new dot [MAX_DOTS_NUM];
    int num_dots = 0;

    float ** distances = nullptr; // указатель на указатель - для динамического двумерного массива попарных расстояний

    distance_index ** sorted_distance_indexes;

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
        calculate_pair_distance(dots, num_dots, distances, num_dots );
        // проверка вывод в экран
        /*
         cout << setw(10) << "Координата X"
             << "  " << setw(10) << "Координата Y" << endl;
        int len = dots.size();
        for (int i = 0; i < len; i++)
        {
            cout << setw(10) << dots[i].x << "  " << setw(10) << dots[i].y << endl;
        }

        */

       // float dist3_4 = dots[2].distance_from(dots[3]);
      //  cout << endl
       //      << dist3_4 << endl;

        // нужен лог файл
        // нужен файл с результатом
    }
    delete [] dots;
    return 0;
}