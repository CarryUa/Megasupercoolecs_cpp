#include "shape.h"
using namespace msce;

msce::Triangle::Triangle()
{
}

VertexList msce::Triangle::get_vertecies() const
{
    return {
        vec2d(0, 0),
        vec2d(a_, 0),
        vec2d(c_, b_),
    };
}

double msce::Triangle::get_perimetr() const
{

    return a_ + b_ + c_;
}

double msce::Triangle::get_area() const
{
    double p = get_perimetr() / 2;
    double area_sqr = p * (p - a_) * (p - b_) * (p - c_);

    return sqrt(area_sqr);
}

double msce::Triangle::get_h() const
{
    return get_perimetr() * 2 / a_;
}
double msce::Triangle::get_w() const
{
    return a_;
}

// msce::Square::Square()
// {
// }

// VertexList msce::Square::get_vertecies() const
// {
//     return {
//         vec2d(0, 0),
//         vec2d(a_, 0),
//         vec2d(a_, a_),
//         vec2d(0, a_),
//     };
// }

// double msce::Square::get_perimetr() const
// {
//     return a_ * 4;
// }

// double msce::Square::get_area() const
// {
//     return a_ * a_;
// }

msce::Rectangle::Rectangle()
{
}

VertexList msce::Rectangle::get_vertecies() const
{
    return {
        vec2d(0, 0),
        vec2d(a_, 0),
        vec2d(a_, b_),
        vec2d(0, b_),
    };
}

double msce::Rectangle::get_perimetr() const
{
    return 2 * (a_ + b_);
}

double msce::Rectangle::get_area() const
{
    return a_ * b_;
}

double msce::Rectangle::get_w() const
{
    return a_;
}

double msce::Rectangle::get_h() const
{
    return b_;
}

msce::BaseShape2D::BaseShape2D()
{
}

VertexList msce::Circle::get_vertecies() const
{
    constexpr double rads_per_vertex_step = (2 * M_PI) / MSCE_CIRCLE_VERTICES_COUNT;
    VertexList result(MSCE_CIRCLE_VERTICES_COUNT);

    for (int i = 0; i < MSCE_CIRCLE_VERTICES_COUNT; i++)
    {
        double current_angle = i * rads_per_vertex_step;

        result[i] = vec2d(sin(current_angle), cos(current_angle)) * r_ + r_;
    }

    return result;
}

double msce::Circle::get_perimetr() const
{
    constexpr double two_pi = 2 * M_PI;
    return two_pi * r_;
}

double msce::Circle::get_area() const
{
    return M_PI * r_ * r_;
}

double msce::Circle::get_w() const
{
    return r_ * 2;
}

double msce::Circle::get_h() const
{
    return r_ * 2;
}
