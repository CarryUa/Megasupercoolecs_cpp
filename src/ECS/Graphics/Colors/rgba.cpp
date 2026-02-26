#include <ECS/Graphics/Colors/rgba.h>
#include <cmath>

RGBAFloat::RGBAFloat(const RGBAByte &byteColor)
{
    r = byteColor.r / 255.0f;
    g = byteColor.g / 255.0f;
    b = byteColor.b / 255.0f;
    a = byteColor.a / 255.0f;
}
RGBAFloat RGBAFloat::operator+(const RGBAFloat &other) const
{
    return RGBAFloat(r + other.r, g + other.g, b + other.b, a + other.a);
}

RGBAByte::RGBAByte(const RGBAFloat &floatColor)
{
    r = (unsigned char)round(floatColor.r * 255.0f);
    g = (unsigned char)round(floatColor.g * 255.0f);
    b = (unsigned char)round(floatColor.b * 255.0f);
    a = (unsigned char)round(floatColor.a * 255.0f);
}
RGBAByte RGBAByte::operator+(const RGBAByte &other) const
{
    return RGBAByte(r + other.r, g + other.g, b + other.b, a + other.a);
}