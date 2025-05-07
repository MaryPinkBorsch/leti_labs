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

void matrix2vector(unsigned long &image_width, unsigned long &image_height, std::vector<double> &out_data, std::vector<std::vector<Pixel>> &in_data)
{
    if (in_data.empty())
        return;

    int counter = 0; // счетчик чтоб бежать по инпуту и записывать цвет каналы по пикселям

    for (int i = 0; i < image_height; i++)
    {
        for (int j = 0; j < image_width; j++)
        {
            if (counter >= in_data.size())
            {
                cout << "ОШИБКА" << endl;
                return;
            }
            out_data.push_back(in_data[i][j].Y);
            out_data.push_back(in_data[i][j].Cb);
            out_data.push_back(in_data[i][j].Cr);
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
//  урезаные значения выставляются в 0 и далее не обрабатываются !!!
//(TODO потом проверить что оно так работает)
//  и еще проверить оно с блоками или с пикселями должно работать?
void downsampling2(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int H)
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
            if ((i % H) == 0 && (j % H) == 0 && j && i)
            {
                // data[i][j].Y = 0;
                data[i][j].Cb = 0;
                data[i][j].Cr = 0;
            }
        }
    }
}

void downsampling(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, unsigned long H)
{
    if (image_height % H != 0 || image_width % H != 0)
    {
        std::cout << "НЕТ возможности сделать даунсемлинг с данным коэффициентом Н, попробуйте другой" << std::endl;
        return;
    }

    unsigned long new_width = image_width / H;
    unsigned long new_height = image_height / H;

    std::vector<std::vector<Pixel>> data_downsampled(new_height, std::vector<Pixel>(new_width));

    for (unsigned long i = 0; i < new_height; ++i)
    {
        for (unsigned long j = 0; j < new_width; ++j)
        {
            data_downsampled[i][j] = data[i * H][j * H]; // Копируем каждый H-й пиксель
        }
    }

    // Заменяем старые данные новыми
    data = data_downsampled;
    image_width = new_width;
    image_height = new_height;
}

void upsampling_bilinear(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, double H)
{
    unsigned long new_width = static_cast<unsigned long>(image_width * H);
    unsigned long new_height = static_cast<unsigned long>(image_height * H);

    std::vector<std::vector<Pixel>> data_upsampled(new_height, std::vector<Pixel>(new_width));

    for (unsigned long i = 0; i < new_height; ++i)
    {
        for (unsigned long j = 0; j < new_width; ++j)
        {
            double x = static_cast<double>(j) / H;
            double y = static_cast<double>(i) / H;

            int x1 = static_cast<int>(x);
            int y1 = static_cast<int>(y);

            int x2 = std::min(x1 + 1, static_cast<int>(image_width) - 1);  // Ensure within bounds
            int y2 = std::min(y1 + 1, static_cast<int>(image_height) - 1); // Ensure within bounds

            double dx = x - x1;
            double dy = y - y1;

            // Интерполируем каждый цветовой компонент (Y, Cb, Cr)
            double Y1 = (1 - dx) * data[y1][x1].Y + dx * data[y1][x2].Y;
            double Y2 = (1 - dx) * data[y2][x1].Y + dx * data[y2][x2].Y;
            data_upsampled[i][j].Y = (1 - dy) * Y1 + dy * Y2;

            double Cb1 = (1 - dx) * data[y1][x1].Cb + dx * data[y1][x2].Cb;
            double Cb2 = (1 - dx) * data[y2][x1].Cb + dx * data[y2][x2].Cb;
            data_upsampled[i][j].Cb = (1 - dy) * Cb1 + dy * Cb2;

            double Cr1 = (1 - dx) * data[y1][x1].Cr + dx * data[y1][x2].Cr;
            double Cr2 = (1 - dx) * data[y2][x1].Cr + dx * data[y2][x2].Cr;
            data_upsampled[i][j].Cr = (1 - dy) * Cr1 + dy * Cr2;
        }
    }

    // Заменяем старые данные новыми
    data = data_upsampled;
    image_width = new_width;
    image_height = new_height;
}

// сюда же пихну обработку по блокам (размер NxN)
void blocking(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int N, std::vector<Block> &blocks)
{
    size_t blocks_width = image_width / N +  ((image_width / N) ? 0 : 1);
    size_t blocks_height = image_height / N +  ((image_height / N) ? 0 : 1);
    blocks.resize(blocks_height*blocks_width);
    for (auto & block : blocks) 
    {
        block.matrix_data.resize(N);
        for (auto & row : block.matrix_data) 
        {
            row.resize(N);
        }
    }

    for (size_t y = 0; y < image_height; ++y) 
    {
        for (size_t x = 0; x < image_height; ++x) 
        {
            size_t blocks_y = y / N;
            size_t blocks_x = x / N;
            size_t blocks_idx = blocks_y*blocks_width + blocks_x;
            blocks[blocks_idx].matrix_data[y % N][x % N] = data[y][x];
        }
    }
}

void deblocking(unsigned long &image_width, unsigned long &image_height, std::vector<std::vector<Pixel>> &data, int N, std::vector<Block> &blocks)
{
    if (N <= 0)
    {
        std::cout << "Ошибка: N должно быть положительным числом." << std::endl;
        return;
    }

    if (blocks.empty())
    {
        std::cout << "Ошибка: Вектор блоков пуст." << std::endl;
        return;
    }

    // Вычисляем размеры исходного изображения (до блокировки).  Важно, чтобы image_width и image_height были корректными
    unsigned long original_width = 0;
    unsigned long original_height = 0;
    if (image_width > 0)
        original_width = image_width / N * N;
    if (image_height > 0)
        original_height = image_height / N * N;

    if (original_width == 0 || original_height == 0)
    {
        std::cout << "Ошибка: image_width или image_height равны 0 или N больше размеров изображения." << std::endl;
        return;
    }

    // Создаем вектор для хранения восстановленного изображения.
    data.resize(original_height);
    for (size_t i = 0; i < original_height; ++i)
    {
        data[i].resize(original_width);
    }

    int block_index = 0;
    for (unsigned long i = 0; i < original_height; i += N)
    {
        for (unsigned long j = 0; j < original_width; j += N)
        {
            // Проверяем, не вышли ли за пределы вектора блоков.  Важно, чтобы количество блоков соответствовало размерам.
            if (block_index >= blocks.size())
            {
                std::cout << "Ошибка: Недостаточно блоков для восстановления изображения." << std::endl;
                return;
            }

            // Копируем данные из блока в изображение.
            for (int row_in_block = 0; row_in_block < N; ++row_in_block)
            {
                for (int col_in_block = 0; col_in_block < N; ++col_in_block)
                {
                    data[i + row_in_block][j + col_in_block] = blocks[block_index].matrix_data[row_in_block][col_in_block];
                }
            }
            block_index++;
        }
    }

    // Обновляем размеры изображения
    image_width = original_width;
    image_height = original_height;

    if (block_index != blocks.size())
    {
        std::cout << "Предупреждение: Количество блоков больше, чем необходимо для восстановления изображения." << std::endl;
    }
}