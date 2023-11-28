#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>

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

bool read_dots(std::string filename, std::vector<dot> & dots)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    // TODO: тут проверить что файл открылся gotovo
    if (input.eof())
    {
        std::cout << " Файл input пуст, упс" << endl;
        return false;
    }
   
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
            while(input.peek() == ' ')
                input >> noskipws >> next_char;
            next_char = input.peek();

            if (next_char != ' ' && next_char != '\r' && next_char != '\n' && dot_x_empty == false)
            {
                input >> skipws >> tmp.y;
                dot_y_empty = false;
                if (dot_x_empty == false && dot_y_empty == false)
                {
                    dots.push_back(tmp);
                }
            }
        }
        while(input.peek() == ' ' || input.peek() == '\r' || input.peek() == '\n')
            input >> noskipws >> next_char;
        
    }
    return true;

}

void calculate_pair_distance(std::vector<dot> & dots, std::vector<std::vector<float>> & distances) 
{
    for(int i = 0; i< dots.size(); i++)
    {   
        distances[i][i] = 0;    
        for(int j = i+1; j< dots.size(); j++)
        {
            distances[i][j] = dots[i].distance_from(dots[j]);
            distances[j][i] = distances[i][j];
        }
    }
}

int main(int argc, char *argv[])
{
    std::cout << "Добро пожаловать в курсовик Калюжной Марии 3352 26.11.23 !" << endl
              << endl;

    string filename = "input.txt";
    std::vector<dot> dots; // создаю вектор-массив для хранения точек (и их координат)
    std::vector<std::vector<float>> distances;

    // загрузить координаты точек из файла
    if (read_dots(filename, dots)) 
    {
        // 1. посчитать все парные расстояния между точками (построить матрицу расстояний между двумя любыми точками)

        // проверка вывод в экран
        cout << setw(10) << "Координата X"
            << "  " << setw(10) << "Координата Y" << endl;
        int len = dots.size();
        for (int i = 0; i < len; i++)
        {
            cout << setw(10) << dots[i].x << "  " << setw(10) << dots[i].y << endl;
        }

        float dist3_4 = dots[2].distance_from(dots[3]);
        cout << endl
            << dist3_4 << endl;

        // нужен лог файл
        // нужен файл с результатом
    }

    return 0;
}