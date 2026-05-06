#include <gtest/gtest.h>
#include <test_configs.h>

using namespace msce;

TEST(ColorTests, CreationTest)
{
    unsigned char rand_a = rand() % 256;

    Color white_hex("#ffffff", rand_a);
    EXPECT_EQ(white_hex.hex(), "ffffff");
    EXPECT_EQ(white_hex.r(), 255);
    EXPECT_EQ(white_hex.g(), 255);
    EXPECT_EQ(white_hex.b(), 255);
    EXPECT_EQ(white_hex.a(), rand_a);

    unsigned char rand_a2 = rand() % 256;
    Color white_hex_lower("fFfFff", rand_a);
    EXPECT_EQ(white_hex.hex(), "ffffff");
    EXPECT_EQ(white_hex.r(), 255);
    EXPECT_EQ(white_hex.g(), 255);
    EXPECT_EQ(white_hex.b(), 255);
    EXPECT_EQ(white_hex.a(), rand_a);

    unsigned char rand_a3 = rand() % 256;
    Color white_vec(Vector3D<unsigned char>(255, 255, 255), rand_a3);
    EXPECT_EQ(white_vec.hex(), "ffffff");
    EXPECT_EQ(white_vec.r(), 255);
    EXPECT_EQ(white_vec.g(), 255);
    EXPECT_EQ(white_vec.b(), 255);
    EXPECT_EQ(white_vec.a(), rand_a3);

    Color white_copy(white_vec);
    EXPECT_EQ(white_vec.hex(), white_copy.hex());
    EXPECT_EQ(white_vec.r(), white_copy.r());
    EXPECT_EQ(white_vec.g(), white_copy.g());
    EXPECT_EQ(white_vec.b(), white_copy.b());
    EXPECT_EQ(white_vec.a(), white_copy.a());
}
TEST(ColorTests, Functions)
{

    Color white_hex("FfFfFF");
    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        unsigned char rand_r = rand() % 256;
        unsigned char rand_g = rand() % 256;
        unsigned char rand_b = rand() % 256;
        unsigned char rand_a = rand() % 256;

        white_hex.r(rand_r);
        EXPECT_EQ(white_hex.r(), rand_r);

        white_hex.g(rand_g);
        EXPECT_EQ(white_hex.g(), rand_g);

        white_hex.b(rand_b);
        EXPECT_EQ(white_hex.b(), rand_b);

        white_hex.a(rand_a);
        EXPECT_EQ(white_hex.a(), rand_a);

        EXPECT_EQ(white_hex.hex(), std::format("{:02x}{:02x}{:02x}", rand_r, rand_g, rand_b, rand_a));
    }
}

TEST(ColorTests, Operations)
{
    unsigned char r1 = rand() % 256;
    unsigned char g1 = rand() % 256;
    unsigned char b1 = rand() % 256;

    std::string hex1 = std::format("{:02x}{:02X}{:02x}", r1, g1, b1);

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {

        Color color1_hex(hex1);

        EXPECT_TRUE(color1_hex == hex1);
        EXPECT_TRUE(color1_hex == Vector3D<unsigned char>(r1, g1, b1));
        EXPECT_TRUE(color1_hex == color1_hex);
        EXPECT_FALSE(color1_hex == "Invalid Hex");
        EXPECT_FALSE(color1_hex == Vector3D<unsigned char>(r1 - 2, g1 + 1, b1 - 1));
    }
}