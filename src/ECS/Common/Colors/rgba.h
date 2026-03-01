#ifndef _MSCE_RGBA_H_
#define _MSCE_RGBA_H_

namespace msce
{

    struct RGBAByte;
    struct RGBAFloat;

    struct RGBAFloat
    {
        float r;
        float g;
        float b;
        float a;

        RGBAFloat operator+(const RGBAFloat &other) const;
        RGBAFloat(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) : r(r), g(g), b(b), a(a) {};
        explicit RGBAFloat(const RGBAByte &byteColor);
    };

    struct RGBAByte
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        RGBAByte operator+(const RGBAByte &other) const;
        RGBAByte(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
        explicit RGBAByte(const RGBAFloat &byteColor);
    };

}

#endif