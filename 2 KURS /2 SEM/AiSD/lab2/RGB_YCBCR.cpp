#include "RGB_YCBCR.h"

#include "math.h"

using namespace std;

void RGB2YCBCR(unsigned char rgb[3], double ycbcr[3])
{
    double fr = (double)rgb[0];
    double fg = (double)rgb[1];
    double fb = (double)rgb[2];

    double Y = 0.2989 * fr + 0.5866 * fg + 0.1145 * fb;
    double Cb = 128.0 - 0.1687 * fr - 0.3313 * fg + 0.5000 * fb;
    double Cr = 128.0 + 0.5000 * fr - 0.4184 * fg - 0.0816 * fb;

    ycbcr[0] = Y;
    ycbcr[1] = Cb;
    ycbcr[2] = Cr;
}

void YCBCR2RGB(double ycbcr[3], unsigned char rgb[3])
{
    double r = ycbcr[0] + 1.4022 * (ycbcr[2] - 128.0);
    double g = ycbcr[0] - 0.3456 * (ycbcr[1] - 128.0) - 0.7145 * (ycbcr[2] - 128.0);
    double b = ycbcr[0] + 1.7710 * (ycbcr[1] - 128.0);
    rgb[0] = (unsigned char)(round(r));
    rgb[1] = (unsigned char)(round(g));
    rgb[2] = (unsigned char)(round(b));
}

void RGB_to_YCBRCR_vector(std::vector<unsigned char> &input, std::vector<double> &output)
{
    output.resize(input.size());
    for (int i = 0; i < input.size(); i += 3)
    {
        // ссылка на Итый элемент приводится к ссылке на массив из 3х чаров
        RGB2YCBCR(reinterpret_cast<unsigned char (&)[3]>(input[i]), reinterpret_cast<double (&)[3]>(output[i]));
    }
}
void YCBRCR_to_RGB_vector(std::vector<double> &input, std::vector<unsigned char> &output)
{
    output.resize(input.size());
    for (int i = 0; i < input.size(); i += 3)
    {
        // ссылка на Итый элемент приводится к ссылке на массив из 3х чаров
        YCBCR2RGB(reinterpret_cast<double (&)[3]>(input[i]), reinterpret_cast<unsigned char (&)[3]>(output[i]));
    }
}
