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

// вот это обраьный семплинг а нужен прямой на обзеку
void redownsampling(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int H)
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
                if ((k % H) - 1 == 0 && (n % H) - 1 == 0 && n && k) // как режем так и восстанавливаем
                {
                    data[k][n].Cb = data[i][j].Cb;
                    data[k][n].Cr = data[i][j].Cr;
                }
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

// эта функция урезает каналы СБ и Ср на столбац и строках, кратных Н
//  урезаные значения выставляются в -1 и далее не обрабатываются !!!
//(TODO потом проверить что оно так работает)
//  и еще проверить оно с блоками или с пикселями должно работать?
void downsampling(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int H)
{
    if (image_height % H != 0 || image_width % H != 0)
    {
        cout << "НЕТ возможности сделать даунсемлинг с данным коэффициентом Н, попробуйте другой" << endl;
        return;
    }

    // int i = 0; // бегать по высоте
    // int j = 0; // бегать по ширине
    for (int i = 0; i < image_height; i++)
    {
        for (int j = 0; j < image_width; ++j)
        {
            if ((i % H) - 1 == 0 && (j % H) - 1 == 0 && j && i)
            {
                data[i][j].Cb = -1;
                data[i][j].Cr = -1;
            }
        }
    }
}

// сюда же пихну обработку по блокам (размер NxN)
void blocking(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int N, std::vector<Block> &blocks)
{
    int h = 0;
    int w = 0;
    int i = 0;
    int j = 0;
    if (N <= 0)
    {
        cout << "error" << endl;
        return;
    }
    if (image_height % N != 0)
        h = round(image_height / N); // округляем последнюю строку и потом зануляем ее
    else
        h = image_height;
    if (image_width % N != 0)
        w = round(image_width / N); // округляем последний столбуц и потом зануляем его
    else
        w = image_width;
    blocks.resize(h);
    // int t = 0;
    // for (int i = 0; i < h; i++)
    // {
    //     int hh = i % N;
    //     if (i && i % N == 0)
    //         t++;
    //     for (int j = 0; j < w; j++)
    //     {
    //         int ww = j % N;
    //         if (j && j % N == 0)
    //             t++;

    //         if (t >= blocks.size() || t >= h * w)
    //             return;
    //         // по заданию надо заполнить пустыми пикселями если четко на блок не делится
    //         if (j > data[i].size() || i > data.size())
    //         {
    //             blocks[t].matrix_data[hh][ww] = Pixel();
    //         }
    //         else
    //         {
    //             blocks[t].matrix_data.resize(N);
    //             blocks[t].matrix_data[hh].resize(N);

    //             blocks[t].matrix_data[hh][ww] = data[i][j];
    //         }
    //     }
    //     if (t)
    //         t -= (w % N) - 1;

    //     if (t < 0)
    //         t = 0;
    // }

    int t = 0; // счеткик по блокам
    // пытаюсь сделвть блочнную обработку (ПЫТАЮСЬ)
    // 1 вайл = 1 блок
    while (i < image_height)
    {
        if (t > (h / N * w / N) - 1)
            break;
        int hh = 0; // указатель бегать внутри блока
        blocks[t].matrix_data.resize(N);
        for (int k = i; k < i + N; k++)
        {
            if (hh > N)
            {
                hh = 0;
            }
            int ww = 0;
            for (int n = j; n < j + N; n++)
            {
                if (n >= image_width)
                {
                    // // заполнить оставшееся нулями
                    // while (n < w)
                    // {
                    //     blocks[t].matrix_data[hh][n] = Pixel();
                    //     n++;
                    // }
                }
                else
                {
                    blocks[t].matrix_data[hh].push_back(data[k][n]);
                    // blocks[t].matrix_data[hh][ww] = data[k][n];}
                    ww++;
                }
            }
            // if (i == image_height)
            // {
            //     // заполнить оставшееся нулями
            //     while (i < h)
            //     {
            //         blocks[t].matrix_data[i][ww] = Pixel();
            //         i++;
            //     }
            // }
            if (j >= image_width)
            {
                // // заполнить оставшееся нулями
                // // заполнить оставшееся нулями
                // while (j < w)
                // {
                //     blocks[t].matrix_data[hh][j] = Pixel();
                //     j++;
                // }

                j = 0;
            }

            hh++;
        }
        t++;
        if (t == w / N)
            i += N;
        j += N;

        if (j >= image_width)
            j = 0;
    }

    // while (i < h)
    // {
    //     while (j < w)
    //     {
    //         if (j % N - 1 == 0)
    //         {
    //             j = 0;
    //             i++;
    //         }

    //         j++;
    //     }
    //     i++;
    // }
}