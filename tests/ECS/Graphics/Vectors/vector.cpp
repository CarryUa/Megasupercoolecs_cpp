#include <gtest/gtest.h>
#include <ECS/Graphics/Vectors/vector.h>

TEST(Vector2DTests, Vector2DComponentConstructorTest)
{
    int ex_x = rand();
    int ex_y = rand();
    Vector2D<int> vec2i(ex_x, ex_y);
    EXPECT_EQ(vec2i.x, ex_x);
    EXPECT_EQ(vec2i.y, ex_y);
}

TEST(Vector2DTests, Vector2DCopyConstructorTest)
{
    int ex_x = rand();
    int ex_y = rand();

    Vector2D<int> vec2i(ex_x, ex_y);
    Vector2D<int> vec2i_copy(vec2i);

    // Copy's components should be same as original's
    EXPECT_EQ(vec2i_copy.x, ex_x);
    EXPECT_EQ(vec2i_copy.y, ex_y);

    // Copy and original addresses should be diferent
    EXPECT_NE(&vec2i, &vec2i_copy);
}

TEST(Vector2DTests, Vector2DScalarMathTest_INT)
{
    for (int i = 0; i < 10; i++)
    {
        int rand_x = rand();
        int rand_y = rand();

        // Prevent division by zero
        int rand_num = rand() % 1000 + 1;

        Vector2D<int> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D(rand_x / rand_num, rand_y / rand_num));
    }
}
TEST(Vector2DTests, Vector2DScalarMathTest_UNSIGNED_INT)
{
    for (int i = 0; i < 10; i++)
    {
        unsigned int rand_x = rand();
        unsigned int rand_y = rand();

        // Prevent division by zero
        unsigned int rand_num = rand() % 1000 + 1;

        Vector2D<unsigned int> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D(rand_x / rand_num, rand_y / rand_num));
    }
}

TEST(Vector2DTests, Vector2DScalarMathTest_FLOAT)
{
    for (int i = 0; i < 10; i++)
    {
        float rand_x = rand();
        float rand_y = rand();

        // Prevent division by zero
        float rand_num = rand() % 1000 + 1;

        Vector2D<float> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D(rand_x / rand_num, rand_y / rand_num));
    }
}

TEST(Vector2DTests, Vector2DScalarMathTest_DOUBLE)
{
    for (int i = 0; i < 10; i++)
    {
        double rand_x = rand();
        double rand_y = rand();

        // Prevent division by zero
        double rand_num = rand() % 1000 + 1;

        Vector2D<double> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D(rand_x / rand_num, rand_y / rand_num));
    }
}

TEST(Vector2DTests, Vector2DScalarMathTest_CHAR)
{
    for (int i = 0; i < 10; i++)
    {
        char rand_x = rand() % 127;
        char rand_y = rand() % 127;

        // Prevent division by zero
        char rand_num = rand() % 10 + 1;

        Vector2D<char> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D<char>(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D<char>(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D<char>(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D<char>(rand_x / rand_num, rand_y / rand_num));
    }
}

TEST(Vector2DTests, Vector2DScalarMathTest_UNSIGNED_CHAR)
{
    for (int i = 0; i < 10; i++)
    {
        unsigned char rand_x = rand() % 255;
        unsigned char rand_y = rand() % 255;

        // Prevent division by zero
        unsigned char rand_num = rand() % 10 + 1;

        Vector2D<unsigned char> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D<unsigned char>(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D<unsigned char>(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D<unsigned char>(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D<unsigned char>(rand_x / rand_num, rand_y / rand_num));
    }
}

TEST(Vector2DTests, Vector2DScalarMathTest_LONG)
{
    for (int i = 0; i < 10; i++)
    {
        long rand_x = rand();
        long rand_y = rand();

        // Prevent division by zero
        long rand_num = rand() % 1000 + 1;

        Vector2D<long> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D(rand_x / rand_num, rand_y / rand_num));
    }
}

TEST(Vector2DTests, Vector2DScalarMathTest_UNSIGNED_LONG)
{
    for (int i = 0; i < 10; i++)
    {
        unsigned long rand_x = rand();
        unsigned long rand_y = rand();

        // Prevent division by zero
        unsigned long rand_num = rand() % 1000 + 1;

        Vector2D<unsigned long> vec1 = Vector2D(rand_x, rand_y);

        // Addition
        EXPECT_EQ(vec1 + rand_num, Vector2D(rand_x + rand_num, rand_y + rand_num));

        // Subtraction
        EXPECT_EQ(vec1 - rand_num, Vector2D(rand_x - rand_num, rand_y - rand_num));

        // Multiplication
        EXPECT_EQ(vec1 * rand_num, Vector2D(rand_x * rand_num, rand_y * rand_num));

        // Division
        EXPECT_EQ(vec1 / rand_num, Vector2D(rand_x / rand_num, rand_y / rand_num));
    }
}
