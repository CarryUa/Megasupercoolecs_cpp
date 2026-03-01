#include <gtest/gtest.h>
#include <test_constants.h>
#include <ECS/Graphics/Vectors/vector.h>
#include <cmath>

using VectorComponentTypes = ::testing::Types<int, unsigned int, float, double,
                                              char, unsigned char, long, unsigned long>;

template <typename VecCT>
class Vector2DTests : public ::testing::Test
{
};

TYPED_TEST_SUITE(Vector2DTests, VectorComponentTypes);

TYPED_TEST(Vector2DTests, ComponentConstructor)
{
    using VecCT = TypeParam;

    VecCT x = static_cast<VecCT>(rand());
    VecCT y = static_cast<VecCT>(rand());

    Vector2D<VecCT> vec(x, y);

    ASSERT_EQ(x, vec.x);
    ASSERT_EQ(y, vec.y);
}

TYPED_TEST(Vector2DTests, CopyConstructor)
{
    using VecCT = TypeParam;

    VecCT x = static_cast<VecCT>(rand());
    VecCT y = static_cast<VecCT>(rand());

    Vector2D<VecCT> vec(x, y);

    Vector2D<VecCT> vec2(vec);

    ASSERT_EQ(vec2.x, vec.x);
    ASSERT_EQ(vec2.y, vec.y);

    ASSERT_NE(&vec, &vec2);
}

#pragma region Scalar Math
TYPED_TEST(Vector2DTests, ScalarMathOperations)
{
    using VecCT = TypeParam;

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {

        VecCT x = static_cast<VecCT>(rand());
        VecCT y = static_cast<VecCT>(rand());

        VecCT rand_num = static_cast<VecCT>(rand());
        if (rand_num == 0)
            rand_num++;

        Vector2D<VecCT> vec(x, y);

        // Addition
        EXPECT_EQ(vec + rand_num, Vector2D<VecCT>(x + rand_num, y + rand_num));

        // Subtraction
        EXPECT_EQ(vec - rand_num, Vector2D<VecCT>(x - rand_num, y - rand_num));

        // Multiplication
        EXPECT_EQ(vec * rand_num, Vector2D<VecCT>(x * rand_num, y * rand_num));

        // Division
        EXPECT_EQ(vec / rand_num, Vector2D<VecCT>(x / rand_num, y / rand_num));

        // Asign operations
        vec += rand_num;
        EXPECT_EQ(vec, Vector2D<VecCT>(x + rand_num, y + rand_num));
        vec = Vector2D(x, y);

        vec -= rand_num;
        EXPECT_EQ(vec, Vector2D<VecCT>(x - rand_num, y - rand_num));
        vec = Vector2D(x, y);

        vec *= rand_num;
        EXPECT_EQ(vec, Vector2D<VecCT>(x * rand_num, y * rand_num));
        vec = Vector2D(x, y);

        vec /= rand_num;
        EXPECT_EQ(vec, Vector2D<VecCT>(x / rand_num, y / rand_num));
        vec = Vector2D(x, y);
    }
}
#pragma endregion

#pragma region Vector Math
TYPED_TEST(Vector2DTests, VectorMathOperations)
{
    using VecCT = TypeParam;

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {

        VecCT x = static_cast<VecCT>(rand());
        if (x == 0)
            x++;
        VecCT y = static_cast<VecCT>(rand());
        if (y == 0)
            y++;

        VecCT x2 = static_cast<VecCT>(rand());
        if (x2 == 0)
            x2++;

        VecCT y2 = static_cast<VecCT>(rand());
        if (y2 == 0)
            y2++;

        Vector2D<VecCT> vec1(x, y);
        Vector2D<VecCT> vec2(x2, y2);

        // Addition
        EXPECT_EQ(vec1 + vec2, Vector2D<VecCT>(x + x2, y + y2));

        // Subtraction
        EXPECT_EQ(vec1 - vec2, Vector2D<VecCT>(x - x2, y - y2));

        // Multiplication
        EXPECT_EQ(vec1 * vec2, Vector2D<VecCT>(x * x2, y * y2));

        // Division
        EXPECT_EQ(vec1 / vec2, Vector2D<VecCT>(x / x2, y / y2));

        // Asign operations
        vec1 += vec2;
        EXPECT_EQ(vec1, Vector2D<VecCT>(x + x2, y + y2));
        vec1 = Vector2D<VecCT>(x, y);

        vec1 -= vec2;
        EXPECT_EQ(vec1, Vector2D<VecCT>(x - x2, y - y2));
        vec1 = Vector2D<VecCT>(x, y);

        vec1 *= vec2;
        EXPECT_EQ(vec1, Vector2D<VecCT>(x * x2, y * y2));
        vec1 = Vector2D<VecCT>(x, y);

        vec1 /= vec2;
        EXPECT_EQ(vec1, Vector2D<VecCT>(x / x2, y / y2));
        vec1 = Vector2D<VecCT>(x, y);
    }
}
#pragma endregion

#pragma region Other Functions

TYPED_TEST(Vector2DTests, VectorFunctions)
{
    using VecCT = TypeParam;

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        VecCT x = static_cast<VecCT>(rand());
        VecCT y = static_cast<VecCT>(rand());

        VecCT x2 = static_cast<VecCT>(rand());
        VecCT y2 = static_cast<VecCT>(rand());

        Vector2D<VecCT> vec1(x, y);
        Vector2D<VecCT> vec2(x2, y2);

        // Length
        double ex_len = sqrt(static_cast<double>(x) * static_cast<double>(x) +
                             static_cast<double>(y) * static_cast<double>(y));
        EXPECT_NEAR(vec1.length(), ex_len, TEST_FLOATING_POINT_TOLERANCE);

        // Dot
        double ex_dot = static_cast<double>(x) * static_cast<double>(x2) +
                        static_cast<double>(y) * static_cast<double>(y2);
        EXPECT_NEAR(vec1.dot(vec2), ex_dot, TEST_FLOATING_POINT_TOLERANCE);

        // Normalized
        Vector2D<double>
            ex_normalized_vec(static_cast<double>(x) / ex_len, static_cast<double>(y) / ex_len);
        EXPECT_EQ(vec1.normalized(), ex_normalized_vec);

        // Normalized Dot
        double ex_len2 = sqrt(static_cast<double>(x2) * static_cast<double>(x2) +
                              static_cast<double>(y2) * static_cast<double>(y2));

        ex_dot = static_cast<double>(x) / ex_len * static_cast<double>(x2) / ex_len2 +
                 static_cast<double>(y) / ex_len * static_cast<double>(y2) / ex_len2;
        EXPECT_NEAR(vec1.normalized_dot(vec2), ex_dot, TEST_FLOATING_POINT_TOLERANCE);
        EXPECT_GE(vec1.normalized_dot(vec2), -1.0);
        EXPECT_LE(vec1.normalized_dot(vec2), 1.0);
    }
}

#pragma endregion