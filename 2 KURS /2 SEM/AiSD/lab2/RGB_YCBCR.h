#pragma once

// переход из РГБ в УСбСр
class RGB
{
public:
    unsigned char R;
    unsigned char G;
    unsigned char B;

    RGB(unsigned char r, unsigned char g, unsigned char b);
    bool Equals(RGB rgb);
};

class YCbCr
{
public:
    float Y;
    float Cb;
    float Cr;

    YCbCr(float y, float cb, float cr);

    bool Equals(YCbCr ycbcr);
};

static YCbCr RGBToYCbCr(RGB rgb);

static float Min(float a, float b);

static float Max(float a, float b);

static RGB YCbCrToRGB(YCbCr ycbcr);