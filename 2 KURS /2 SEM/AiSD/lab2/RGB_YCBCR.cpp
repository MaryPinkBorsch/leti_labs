#include "RGB_YCBCR.h"

RGB::RGB(unsigned char r, unsigned char g, unsigned char b)
{
	R = r;
	G = g;
	B = b;
}

bool RGB::Equals(RGB rgb)
{
	return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
}

YCbCr::YCbCr(float y, float cb, float cr)
{
	Y = y;
	Cb = cb;
	Cr = cr;
}

bool YCbCr::Equals(YCbCr ycbcr)
{
	return (Y == ycbcr.Y) && (Cb == ycbcr.Cb) && (Cr == ycbcr.Cr);
}

//переход из РГБ в УСБСР
static YCbCr RGBToYCbCr(RGB rgb)
{
    float fr = (float)rgb.R / 255;
    float fg = (float)rgb.G / 255;
    float fb = (float)rgb.B / 255;

    float Y = (float)(0.2989 * fr + 0.5866 * fg + 0.1145 * fb);
    float Cb = (float)(-0.1687 * fr - 0.3313 * fg + 0.5000 * fb);
    float Cr = (float)(0.5000 * fr - 0.4184 * fg - 0.0816 * fb);

    return YCbCr(Y, Cb, Cr);
}

static float Min(float a, float b)
{
    return a <= b ? a : b;
}

static float Max(float a, float b)
{
    return a >= b ? a : b;
}

//переход обратно в РГБ
static RGB YCbCrToRGB(YCbCr ycbcr)
{
    float r = Max(0.0f, Min(1.0f, (float)(ycbcr.Y + 0.0000 * ycbcr.Cb + 1.4022 * ycbcr.Cr)));
    float g = Max(0.0f, Min(1.0f, (float)(ycbcr.Y - 0.3456 * ycbcr.Cb - 0.7145 * ycbcr.Cr)));
    float b = Max(0.0f, Min(1.0f, (float)(ycbcr.Y + 1.7710 * ycbcr.Cb + 0.0000 * ycbcr.Cr)));

    return RGB((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255));
}