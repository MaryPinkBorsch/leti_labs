#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// максимально допустимый размер стороны матрицы
const unsigned int MAX_N = 32;

// двумерный статический массив в котором первое измерение\индекс это номер строчки матрицы
// а второе измерение\индекс это номер столбца в матрице
float A[MAX_N][MAX_N];
// во второй двумерный массив записывается результат обработки (по спирали)
float B[MAX_N][MAX_N];

// фактический размер матрицы, считывается из первого числа в файле далее
unsigned int N = 0;

// функция для считывания стоки матрицы, возвращает количество считанных элементов
int read_matrix_line(ifstream &is, float A[MAX_N])
{ // num_elements - кол-во элементов в 1й строке
    int num_elements = 0;
    // обрабатываем строки посимвольно. пропускаем пробелы, когда встречаем перенос строки -
    // считываем его и выходим рано
    while (!is.eof() && num_elements < N && num_elements < MAX_N)
    {
        char next_char;
        next_char = is.peek();
        switch (next_char)
        {
        case ' ': // пробел
            is >> noskipws >> next_char;
            break;
        case '\r': // перенос строки под виндовс "\r\n", я ненавижу 11ю Винду
            is >> noskipws >> next_char;
            is >> noskipws >> next_char;
            return num_elements;
        default: // все остальное числа, считываем целиком
            is >> skipws >> A[num_elements++];
        }
    }
    // все лишнее что осталось тоже пропускаем чтобы не попало в следующую строчку,
    //  типо отматываем до числа
    char next_char = is.peek();
    while (!is.eof() && next_char == ' ' || next_char == '\r' || next_char == '\n')
    {
        is >> noskipws >> next_char;
        next_char = is.peek();
    }

    return num_elements;
}

bool read_matrix(std::string filename)
{
    N = 0;
    ifstream is(filename, ios::in | ios::ate); // открываем файл в конце чтобы проверить его длинну

    if (is.eof() || is.tellg() == 0)
    {
        cout << "ОШИБКА, ПУСТОЙ ФАЙЛ " << filename << endl;
        return false;
    }
    // перемотка в начало
    is.seekg(0);

    is >> skipws >> N; // сначала считаем сторону матрицы
    char next_char = is.peek();
    // надо пропустить все лишнее до первого числа
    while (!is.eof() && next_char == ' ' || next_char == '\r' || next_char == '\n')
    {
        is >> noskipws >> next_char;
        next_char = is.peek();
    }
    for (int i = 0; i < N; ++i)
    {
        // считываем матрицу построчно
        int elements_in_line = read_matrix_line(is, A[i]);
        // если недосчитались элементов в строке сокращаем ранг матрицы
        if (elements_in_line < N)
            N = elements_in_line < i ? i : elements_in_line;
    }
    return true;
}

// возвращает true если надо продолжать обработку
bool process_top_to_bottom(int &top, int &bottom, int &left, int &right, float A[MAX_N][MAX_N], float B[MAX_N][MAX_N], int &counter)
{
    // против часовой стрелки: весь 1й столбец
    for (int i = top; i <= bottom; ++i)
    {
        B[counter / N][counter % N] = A[i][left];
        ++counter;
    }
    ++left;
    if ((left > right) && (bottom < top))
        return false;
    return true;
}
bool process_left_to_right(int &top, int &bottom, int &left, int &right, float A[MAX_N][MAX_N], float B[MAX_N][MAX_N], int &counter)
{
    for (int i = left; i <= right; ++i)
    {
        B[counter / N][counter % N] = A[bottom][i];
        ++counter;
    }
    --bottom;
    if ((left > right) && (bottom < top))
        return false;
    return true;
}
bool process_bottom_to_top(int &top, int &bottom, int &left, int &right, float A[MAX_N][MAX_N], float B[MAX_N][MAX_N], int &counter)
{
    for (int i = bottom; i >= top; --i)
    {
        B[counter / N][counter % N] = A[i][right];
        ++counter;
    }
    --right;
    if ((left > right) && (bottom < top))
        return false;
    return true;
}
bool process_right_to_left(int &top, int &bottom, int &left, int &right, float A[MAX_N][MAX_N], float B[MAX_N][MAX_N], int &counter)
{
    for (int i = right; i >= left; --i)
    {
        B[counter / N][counter % N] = A[top][i];
        ++counter;
    }
    ++top;
    if ((left > right) && (bottom < top))
        return false;
    return true;
}

void process()
{
    // счетчик для записи в результат
    int counter = 0;
    // переменные для обхода по спирали
    int left, right, top, bottom;
    left = 0;
    top = 0;
    right = N - 1;
    bottom = N - 1;

    while (true)
    {
        // против часовой стрелки: весь 1й столбец
        if (!process_top_to_bottom(top, bottom, left, right, A, B, counter))
            break;

        // оставшиеся элементы последней строки
        if (!process_left_to_right(top, bottom, left, right, A, B, counter))
            break;

        // в обратном порядке все оставшиеся элементы последнего столбца
        if (!process_bottom_to_top(top, bottom, left, right, A, B, counter))
            break;

        // в обратном порядке оставшиеся элементы первой строки
        if (!process_right_to_left(top, bottom, left, right, A, B, counter))
            break;
    }
}

void write_matrix_line(ofstream &of, float *MATRIX_LINE, int N)
{
    for (int i = 0; i < N; ++i)
    {
        of << setw(8) << MATRIX_LINE[i] << (i == N - 1 ? "" : " ");
    }
    of << endl;
}

void write_matrix(std::string filename)
{
    ofstream of(filename, ios::out | ios::trunc);
    of << "Результат: ";
    of << endl;
    for (int i = 0; i < N; ++i)
        write_matrix_line(of, B[i] /*начало итой строчки*/, N);

    of << endl;
    of << endl;
    of << "Исходник: ";
    of << endl;
    for (int i = 0; i < N; ++i)
    {
        write_matrix_line(of, A[i] /*начало итой строчки*/, N);
    }
    of << endl;
    of << endl;
    of << "Storona matrix: " << N;
}

/*void print_matrix(std::string filename)
{
    ofstream of(filename, ios::out);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

*/

void convert(string inputfile, string outputfile)
{
    if (read_matrix(inputfile))
    {
        process();
        write_matrix(outputfile);
    }
    else
    {
        ofstream of(outputfile, ios::out | ios::trunc);
        of << "ОШИБКА, ПУСТОЙ ФАЙЛ " << inputfile << endl;
    }
}

int main(int argc, char *argv[])
{
    convert("input.txt", "output.txt");
    convert("input1.txt", "output1.txt");
    convert("input2.txt", "output2.txt");
    convert("input3.txt", "output3.txt");
    convert("input4.txt", "output4.txt");
    convert("input5.txt", "output5.txt");
    convert("input6.txt", "output6.txt");
    convert("input7.txt", "output7.txt");
    convert("input8.txt", "output8.txt");
    convert("input9.txt", "output9.txt");
    convert("input10.txt", "output10.txt");
    return 0;
}