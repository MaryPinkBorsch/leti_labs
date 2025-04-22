#include "downsampling.h"

using namespace std;

void vector_2matrix(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &out_data, std::vector<double> in_data)
{
    if (in_data.empty())
        return;

    int counter = 0; // счетчик чтоб бежать по инпуту и записывать цвет каналы по пикселям
    out_data.resize(image_height);
    for (int i = 0; i < image_height; i++)
    {
        out_data[i].resize(image_width);
        for (int j = 0; j < image_width; j++)
        {
            if (counter >= in_data.size())
            {
                cout << "ОШИБКА" << endl;
                return;
            }
            out_data[i][j].Y = in_data[counter++];
            out_data[i][j].Cb = in_data[counter++];
            out_data[i][j].Cr = in_data[counter++];
        }
    }
}

// дата это наш матрица пикселей, Н это то во сколько надо сжать наше изображение
// нужно проверить что и ширина и высота кратны Н (иначе нормально не засемплить)

// !!! надо семплить ТОЛЬКО СБ и СР каналы!!!!
void downsampling(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int H)
{
    if (image_height % H != 0 || image_width % H != 0)
    {
        cout << "НЕТ возможности сделать даунсемлинг с данным коэффициентом Н, попробуйте другой" << endl;
        return;
    }

    int i = 0; // бегать по высоте
    int j = 0; // бегать по ширине

    // пытаюсь сделвть блочнную обработку (ПЫТАЮСЬ)
    while (i < image_height)
    {
        for (int k = i; k < i + H; k++)
        {
            for (int n = j; n < j + H; n++)
            {
                if (n >= image_width)
                    abort();
                data[k][n].Cb = data[i][j].Cb;
                data[k][n].Cr = data[i][j].Cr;
            }
        }

        j += H;
        if (j == image_width)
        {
            j = 0;
            i += H;
        }
    }
}

// сюда же пихну обработку по блокам (размер NxN)
void blocking(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int N, std::vector<Block> &blocks)
{
    int h = 0;
    int w = 0;
    if (image_height % N != 0)
        h = round(image_height / N); // округляем последнюю строку и зануляем ее
    else
        h = image_height;
    if (image_width % N != 0)
        w = round(image_width / N); // округляем последний столбуц и зануляем его
    else
        w = image_width;
    blocks.resize(h);
    int t = 0;
    for (int i = 0; i < h; i++)
    {
        int hh = i % N;
        if (t && i % N != 0)
            t -= (w % N) - 1;

        if (t < 0)
            t = 0;

        for (int j = 0; j < w; j++)
        {
            int ww = j % N;
            if (j && j % N == 0)
                t++;

            if (t >= blocks.size() || t >= h * w)
                return;
            // по заданию надо заполнить пустыми пикселями если четко на блок не делится
            if (j > data[i].size() || i > data.size())
            {
                blocks[t].matrix_data[hh][ww] = Pixel();
            }
            else
            {
                blocks[t].matrix_data.resize(N);
                blocks[t].matrix_data[hh].resize(N);

                blocks[t].matrix_data[hh][ww] = data[i][j];
            }
        }
    }
}