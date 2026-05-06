#include <gtest/gtest.h>
#include <test_configs.h>
#include <MSCE/Common/vector.h>
#include <cmath>

using namespace msce;
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

#pragma region Vector
static auto logger = Logger("testing");

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

#pragma region Vector3D Tests

template <typename VecCT>
class Vector3DTests : public ::testing::Test
{
};

TYPED_TEST_SUITE(Vector3DTests, VectorComponentTypes);

TYPED_TEST(Vector3DTests, ComponentConstructor)
{
    using VecCT = TypeParam;

    VecCT x = static_cast<VecCT>(rand());
    VecCT y = static_cast<VecCT>(rand());
    VecCT z = static_cast<VecCT>(rand());

    Vector3D<VecCT> vec(x, y, z);

    ASSERT_EQ(x, vec.x);
    ASSERT_EQ(y, vec.y);
    ASSERT_EQ(z, vec.z);
}

TYPED_TEST(Vector3DTests, CopyConstructor)
{
    using VecCT = TypeParam;

    VecCT x = static_cast<VecCT>(rand());
    VecCT y = static_cast<VecCT>(rand());
    VecCT z = static_cast<VecCT>(rand());

    Vector3D<VecCT> vec(x, y, z);

    Vector3D<VecCT> vec2(vec);

    ASSERT_EQ(vec2.x, vec.x);
    ASSERT_EQ(vec2.y, vec.y);
    ASSERT_EQ(vec2.z, vec.z);

    ASSERT_NE(&vec, &vec2);
}

#pragma region Vector3D Scalar Math
TYPED_TEST(Vector3DTests, ScalarMathOperations)
{
    using VecCT = TypeParam;

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {

        VecCT x = static_cast<VecCT>(rand());
        VecCT y = static_cast<VecCT>(rand());
        VecCT z = static_cast<VecCT>(rand());

        VecCT rand_num = static_cast<VecCT>(rand());
        if (rand_num == 0)
            rand_num++;

        Vector3D<VecCT> vec(x, y, z);

        // Addition
        EXPECT_EQ(vec + rand_num, Vector3D<VecCT>(x + rand_num, y + rand_num, z + rand_num));

        // Subtraction
        EXPECT_EQ(vec - rand_num, Vector3D<VecCT>(x - rand_num, y - rand_num, z - rand_num));

        // Multiplication
        EXPECT_EQ(vec * rand_num, Vector3D<VecCT>(x * rand_num, y * rand_num, z * rand_num));

        // Division
        EXPECT_EQ(vec / rand_num, Vector3D<VecCT>(x / rand_num, y / rand_num, z / rand_num));

        // Assign operations
        vec += rand_num;
        EXPECT_EQ(vec, Vector3D<VecCT>(x + rand_num, y + rand_num, z + rand_num));
        vec = Vector3D(x, y, z);

        vec -= rand_num;
        EXPECT_EQ(vec, Vector3D<VecCT>(x - rand_num, y - rand_num, z - rand_num));
        vec = Vector3D(x, y, z);

        vec *= rand_num;
        EXPECT_EQ(vec, Vector3D<VecCT>(x * rand_num, y * rand_num, z * rand_num));
        vec = Vector3D(x, y, z);

        vec /= rand_num;
        EXPECT_EQ(vec, Vector3D<VecCT>(x / rand_num, y / rand_num, z / rand_num));
        vec = Vector3D(x, y, z);
    }
}
#pragma endregion

#pragma region Vector3D Vector Math

TYPED_TEST(Vector3DTests, VectorMathOperations)
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
        VecCT z = static_cast<VecCT>(rand());
        if (z == 0)
            z++;

        VecCT x2 = static_cast<VecCT>(rand());
        if (x2 == 0)
            x2++;

        VecCT y2 = static_cast<VecCT>(rand());
        if (y2 == 0)
            y2++;

        VecCT z2 = static_cast<VecCT>(rand());
        if (z2 == 0)
            z2++;

        Vector3D<VecCT> vec1(x, y, z);
        Vector3D<VecCT> vec2(x2, y2, z2);

        // Addition
        EXPECT_EQ(vec1 + vec2, Vector3D<VecCT>(x + x2, y + y2, z + z2));

        // Subtraction
        EXPECT_EQ(vec1 - vec2, Vector3D<VecCT>(x - x2, y - y2, z - z2));

        // Multiplication
        EXPECT_EQ(vec1 * vec2, Vector3D<VecCT>(x * x2, y * y2, z * z2));

        // Division
        EXPECT_EQ(vec1 / vec2, Vector3D<VecCT>(x / x2, y / y2, z / z2));

        // Assign operations
        vec1 += vec2;
        EXPECT_EQ(vec1, Vector3D<VecCT>(x + x2, y + y2, z + z2));
        vec1 = Vector3D<VecCT>(x, y, z);

        vec1 -= vec2;
        EXPECT_EQ(vec1, Vector3D<VecCT>(x - x2, y - y2, z - z2));
        vec1 = Vector3D<VecCT>(x, y, z);

        vec1 *= vec2;
        EXPECT_EQ(vec1, Vector3D<VecCT>(x * x2, y * y2, z * z2));
        vec1 = Vector3D<VecCT>(x, y, z);

        vec1 /= vec2;
        EXPECT_EQ(vec1, Vector3D<VecCT>(x / x2, y / y2, z / z2));
        vec1 = Vector3D<VecCT>(x, y, z);
    }
}
#pragma endregion

#pragma region Vector3D Other Functions

TYPED_TEST(Vector3DTests, VectorFunctions)
{
    using VecCT = TypeParam;

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        VecCT x = static_cast<VecCT>(rand());
        VecCT y = static_cast<VecCT>(rand());
        VecCT z = static_cast<VecCT>(rand());

        VecCT x2 = static_cast<VecCT>(rand());
        VecCT y2 = static_cast<VecCT>(rand());
        VecCT z2 = static_cast<VecCT>(rand());

        Vector3D<VecCT> vec1(x, y, z);
        Vector3D<VecCT> vec2(x2, y2, z2);

        // Length
        double ex_len = sqrt(static_cast<double>(x) * static_cast<double>(x) +
                             static_cast<double>(y) * static_cast<double>(y) +
                             static_cast<double>(z) * static_cast<double>(z));
        EXPECT_NEAR(vec1.length(), ex_len, TEST_FLOATING_POINT_TOLERANCE);

        // Dot
        double ex_dot = static_cast<double>(x) * static_cast<double>(x2) +
                        static_cast<double>(y) * static_cast<double>(y2) +
                        static_cast<double>(z) * static_cast<double>(z2);
        EXPECT_NEAR(vec1.dot(vec2), ex_dot, TEST_FLOATING_POINT_TOLERANCE);

        // Normalized
        Vector3D<double>
            ex_normalized_vec(static_cast<double>(x) / ex_len, static_cast<double>(y) / ex_len, static_cast<double>(z) / ex_len);
        EXPECT_EQ(vec1.normalized(), ex_normalized_vec);

        // Normalized Dot
        double ex_len2 = sqrt(static_cast<double>(x2) * static_cast<double>(x2) +
                              static_cast<double>(y2) * static_cast<double>(y2) +
                              static_cast<double>(z2) * static_cast<double>(z2));

        ex_dot = static_cast<double>(x) / ex_len * static_cast<double>(x2) / ex_len2 +
                 static_cast<double>(y) / ex_len * static_cast<double>(y2) / ex_len2 +
                 static_cast<double>(z) / ex_len * static_cast<double>(z2) / ex_len2;
        EXPECT_NEAR(vec1.normalized_dot(vec2), ex_dot, TEST_FLOATING_POINT_TOLERANCE);
        EXPECT_GE(vec1.normalized_dot(vec2), -1.0);
        EXPECT_LE(vec1.normalized_dot(vec2), 1.0);
    }
}

#pragma endregion