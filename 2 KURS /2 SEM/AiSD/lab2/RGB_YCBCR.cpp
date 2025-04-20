#include "RGB_YCBCR.h"

using namespace std;

void RGB2YCBCR(unsigned char rgb[3], double ycbcr[3])
{
    float fr = (double)rgb[0] / 255;
    float fg = (double)rgb[1] / 255;
    float fb = (double)rgb[2] / 255;

    float Y = (double)(0.2989 * fr + 0.5866 * fg + 0.1145 * fb);
    float Cb = (double)(-0.1687 * fr - 0.3313 * fg + 0.5000 * fb);
    float Cr = (double)(0.5000 * fr - 0.4184 * fg - 0.0816 * fb);

    ycbcr[0] = Y;
    ycbcr[1] = Cb;
    ycbcr[2] = Cr;
}

void YCBCR2RGB(double ycbcr[3], unsigned char rgb[3])
{
    double r = std::max(0.0, std::min(1.0, (double)(ycbcr[0] + 0.0000 * ycbcr[1] + 1.4022 * ycbcr[2])));
    double g = std::max(0.0, std::min(1.0, (double)(ycbcr[0] - 0.3456 * ycbcr[1] - 0.7145 * ycbcr[2])));
    double b = std::max(0.0, std::min(1.0, (double)(ycbcr[0] + 1.7710 * ycbcr[1] + 0.0000 * ycbcr[2])));
    rgb[0] = (unsigned char)(r * 255);
    rgb[1] = (unsigned char)(g * 255);
    rgb[2] = (unsigned char)(b * 255);
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
