#include <ECS/Common/Colors/rgba.h>
#include <gtest/gtest.h>

using namespace msce;

TEST(RGBAFloatTest, ConstructorTest)
{
    RGBAFloat color(0.5f, 0.25f, 0.75f, 1.0f);
    EXPECT_FLOAT_EQ(color.r, 0.5f);
    EXPECT_FLOAT_EQ(color.g, 0.25f);
    EXPECT_FLOAT_EQ(color.b, 0.75f);
    EXPECT_FLOAT_EQ(color.a, 1.0f);
}
TEST(RGBAFloatTest, ToRGBAByteCastTest)
{
    RGBAFloat color(0.5f, 0.25f, 0.75f, 1.0f);
    RGBAByte byteColor = (RGBAByte)color;
    EXPECT_EQ(byteColor.r, (unsigned char)128);
    EXPECT_EQ(byteColor.g, (unsigned char)64);
    EXPECT_EQ(byteColor.b, (unsigned char)191);
    EXPECT_EQ(byteColor.a, (unsigned char)255);
}
TEST(RGBAByteTest, ConstructorTest)
{
    RGBAByte color(128, 64, 191, 255);
    EXPECT_EQ(color.r, (unsigned char)128);
    EXPECT_EQ(color.g, (unsigned char)64);
    EXPECT_EQ(color.b, (unsigned char)191);
    EXPECT_EQ(color.a, (unsigned char)255);
}
TEST(RGBAByteTest, ToRGBAFloatCastTest)
{
    RGBAByte color(128, 64, 191, 255);
    RGBAFloat byteColor = (RGBAFloat)color;
    EXPECT_FLOAT_EQ(byteColor.r, 128 / 255.0f);
    EXPECT_FLOAT_EQ(byteColor.g, 64 / 255.0f);
    EXPECT_FLOAT_EQ(byteColor.b, 191 / 255.0f);
    EXPECT_FLOAT_EQ(byteColor.a, 255 / 255.0f);
}