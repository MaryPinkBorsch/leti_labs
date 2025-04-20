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