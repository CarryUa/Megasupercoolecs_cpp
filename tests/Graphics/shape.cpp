#include <gtest/gtest.h>
#include <test_configs.h>

#include <algorithm>
#include <cmath>

#include <MSCE/Graphics/shape.h>

using namespace msce;

constexpr double pi_180th = M_PI / 180;

TEST(Geometry2DTests, Triangles)
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

TEST(Geometry2DTests, Circles)
{
    constexpr double two_pi = M_PI * 2;
    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        Circle default_circle;
        Circle copy(default_circle);

        EXPECT_EQ(default_circle.radius(), copy.radius());
        EXPECT_EQ(default_circle.get_area(), copy.get_area());
        EXPECT_EQ(default_circle.get_perimetr(), copy.get_perimetr());
        EXPECT_EQ(default_circle.get_h(), copy.get_h());
        EXPECT_EQ(default_circle.get_w(), copy.get_w());
        EXPECT_EQ(default_circle.get_w(), copy.get_h());

        double rand_radius = (rand() % 500) / 3;
        Circle c1(rand_radius);
        EXPECT_EQ(c1.radius(), rand_radius);
        EXPECT_NEAR(c1.get_area(), M_PI * rand_radius * rand_radius, TEST_FLOATING_POINT_TOLERANCE);
        EXPECT_NEAR(c1.get_perimetr(), two_pi * rand_radius, TEST_FLOATING_POINT_TOLERANCE);
    }
}
TEST(Geometry2DTests, Rectangles)
{
    for (size_t i = 0; i < TEST_ITERATIONS; i++)
    {
        Rectangle rect_default;
        Rectangle copy(rect_default);
        EXPECT_EQ(rect_default.get_area(), copy.get_area());
        EXPECT_EQ(rect_default.get_h(), copy.get_h());
        EXPECT_EQ(rect_default.get_w(), copy.get_w());
        EXPECT_EQ(rect_default.get_perimetr(), copy.get_perimetr());

        double rand_a = (rand() % 500) / 3, rand_b = (rand() % 500) / 3;
        Rectangle rect1(rand_a, rand_b);
        EXPECT_EQ(rect1.get_area(), rand_a * rand_b);
        EXPECT_EQ(rect1.get_perimetr(), (rand_a + rand_b) * 2);
        EXPECT_EQ(rect1.a(), rand_a);
        EXPECT_EQ(rect1.b(), rand_b);
    }
}