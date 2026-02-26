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

TEST(Vector2DTests, Vector2DScalarPlusEqualOperatorTest)
{
    int ex_x = rand();
    int ex_y = rand();

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_plus = rand();
    vec2i += rand_plus;

    // Bouth components should grow by same amount
    EXPECT_EQ(vec2i.x, ex_x + rand_plus);
    EXPECT_EQ(vec2i.y, ex_y + rand_plus);

    // Address shouldn't change(no new instancing)
    EXPECT_EQ(&(vec2i += rand_plus), &vec2i) << "The original object and result have different addresses.";
}

TEST(Vector2DTests, Vector2DScalarPlusOperatorTest)
{
    int ex_x = rand();
    int ex_y = rand();

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_plus = rand();

    Vector2D vec2i_2 = vec2i + rand_plus;

    EXPECT_EQ(vec2i + rand_plus, Vector2D(ex_x + rand_plus, ex_y + rand_plus));
    EXPECT_NE(&vec2i_2, &vec2i) << "The operation should create new Vector2D instance.";
}

TEST(Vector2DTests, Vector2DScalarMinusEqualOperatorTest)
{
    int ex_x = rand();
    int ex_y = rand();

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_minus = rand();
    vec2i -= rand_minus;

    // Both components should shrink by same amount
    EXPECT_EQ(vec2i.x, ex_x - rand_minus);
    EXPECT_EQ(vec2i.y, ex_y - rand_minus);

    // Address shouldn't change(no new instancing)
    EXPECT_EQ(&(vec2i -= rand_minus), &vec2i) << "The original object and result have different addresses.";
}

TEST(Vector2DTests, Vector2DScalarMinusOperatorTest)
{
    int ex_x = rand();
    int ex_y = rand();

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_minus = rand();

    Vector2D<int> vec2i_2 = vec2i - rand_minus;

    EXPECT_EQ(vec2i - rand_minus, Vector2D<int>(ex_x - rand_minus, ex_y - rand_minus));
    EXPECT_NE(&vec2i_2, &vec2i) << "The operation should create new Vector2D instance.";
}

TEST(Vector2DTests, Vector2DScalarMultiplyEqualOperatorTest)
{
    int ex_x = rand() % 100 + 1;  // Avoid zero
    int ex_y = rand() % 100 + 1;

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_multiply = rand() % 10 + 1;  // Avoid zero
    vec2i *= rand_multiply;

    // Both components should multiply by same amount
    EXPECT_EQ(vec2i.x, ex_x * rand_multiply);
    EXPECT_EQ(vec2i.y, ex_y * rand_multiply);

    // Address shouldn't change(no new instancing)
    EXPECT_EQ(&(vec2i *= rand_multiply), &vec2i) << "The original object and result have different addresses.";
}

TEST(Vector2DTests, Vector2DScalarMultiplyOperatorTest)
{
    int ex_x = rand() % 100 + 1;
    int ex_y = rand() % 100 + 1;

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_multiply = rand() % 10 + 1;

    Vector2D<int> vec2i_2 = vec2i * rand_multiply;

    EXPECT_EQ(vec2i * rand_multiply, Vector2D<int>(ex_x * rand_multiply, ex_y * rand_multiply));
    EXPECT_NE(&vec2i_2, &vec2i) << "The operation should create new Vector2D instance.";
}

TEST(Vector2DTests, Vector2DScalarDivideEqualOperatorTest)
{
    int ex_x = rand() % 100 + 1;
    int ex_y = rand() % 100 + 1;

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_divide = rand() % 10 + 1;  // Avoid division by zero
    vec2i /= rand_divide;

    // Both components should divide by same amount
    EXPECT_EQ(vec2i.x, ex_x / rand_divide);
    EXPECT_EQ(vec2i.y, ex_y / rand_divide);

    // Address shouldn't change(no new instancing)
    EXPECT_EQ(&(vec2i /= rand_divide), &vec2i) << "The original object and result have different addresses.";
}

TEST(Vector2DTests, Vector2DScalarDivideOperatorTest)
{
    int ex_x = rand() % 100 + 1;
    int ex_y = rand() % 100 + 1;

    Vector2D<int> vec2i(ex_x, ex_y);

    int rand_divide = rand() % 10 + 1;

    Vector2D<int> vec2i_2 = vec2i / rand_divide;

    EXPECT_EQ(vec2i / rand_divide, Vector2D<int>(ex_x / rand_divide, ex_y / rand_divide));
    EXPECT_NE(&vec2i_2, &vec2i) << "The operation should create new Vector2D instance.";
}
