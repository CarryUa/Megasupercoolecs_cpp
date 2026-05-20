#include <gtest/gtest.h>
#include <test_configs.h>

#include <algorithm>
#include <cmath>

#include <MSCE/Graphics/shape.h>

using namespace msce;

constexpr double pi_180th = M_PI / 180;

TEST(Geometry2DTests, ShapeCreationTest)
{
    // default triangle and its copy
    Triangle t_default;
    Triangle t_copy(t_default);

    // EXPECT_EQ(t_default, t_copy);
    EXPECT_EQ(t_default.get_vertecies(), t_copy.get_vertecies());
    EXPECT_EQ(t_default.get_perimetr(), t_copy.get_perimetr());
    EXPECT_EQ(t_default.get_area(), t_copy.get_area());
    EXPECT_EQ(t_default.get_w(), t_copy.get_w());
    EXPECT_EQ(t_default.get_h(), t_copy.get_h());

    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {

        const double a = 1.0 + (rand() % 500) / 100.0;
        const double b = 1.0 + (rand() % 500) / 100.0;
        const double angle_A = (1.0 + (rand() % 180) / 5);

        const double c = std::sqrt(a * a + b * b - 2.0 * a * b * cos(angle_A * pi_180th));
        const double angle_B = acos((c * c + a * a - b * b) / (2 * a * c)) / pi_180th;
        const double angle_C = 180 - angle_A - angle_B;

        const Triangle t_sas = Triangle::create_sas(a, angle_A, b);
        const Triangle t_sss = Triangle::create_sss(a, b, c);
        const Triangle t_asa = Triangle::create_asa(angle_A, a, angle_B);

        EXPECT_NEAR(t_sas.get_perimetr(), a + b + c, TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_sss.get_perimetr(), a + b + c, TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_asa.get_perimetr(), a + b + c, TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);

        const double area = 0.5 * a * b * std::sin(angle_A * pi_180th);
        EXPECT_NEAR(t_sas.get_area(), area, TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_sss.get_area(), area, TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_asa.get_area(), area, TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);

        EXPECT_NEAR(t_sas.get_w(), t_sss.get_w(), TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_sas.get_w(), t_asa.get_w(), TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_sas.get_h(), t_sss.get_h(), TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
        EXPECT_NEAR(t_sas.get_h(), t_asa.get_h(), TEST_FLOATING_POINT_TRIGONOMETRY_TOLERANCE);
    }
}