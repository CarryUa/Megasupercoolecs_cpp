#include <MSCE/Types/shape.h>
#include <MSCE/logger.h>
using namespace msce;

namespace
{
constexpr double pi_180th = M_PI / 180.0;

constexpr double deg_to_rad(const double deg) { return deg * pi_180th; }

constexpr double rad_to_deg(const double rad) { return rad / pi_180th; }

Logger logger("Triangle");
} // namespace

void msce::Triangle::update_vertecies() noexcept {}

msce::Triangle::Triangle() {}

Triangle msce::Triangle::create_sss(double a, double b, double c)
{
  if (a + b < c)
    logger.log_warning("Created invalid triangle(using SSS method) with "
                       "invalid sizes. A({}) + B({}) < C({})",
                       a, b, c);

  Triangle t;
  t.mesh_.vertecies[0] = vec2d(0, 0);
  t.mesh_.vertecies[2] = vec2d(a, 0);

  // c2 = a2 + b2 - 2ab * cos(O)
  // cos(O) = a2 + b2 - 2ab / c2

  double angle_between_a_and_c = acos((a * a + b * b - c * c) / (2 * a * b));
  t.mesh_.vertecies[1] =
      vec2d::create_unit_from_radians(angle_between_a_and_c) * b;

  t.update_vertecies();
  return t;
}

Triangle msce::Triangle::create_sas(double a, double degrees, double b)
{
  if (degrees >= 180.0)
    logger.log_warning(
        "Created invalid triangle(using SAS method) with single angle {}deg.",
        degrees);

  Triangle t;
  t.mesh_.vertecies[0] = vec2d(0, 0);
  t.mesh_.vertecies[2] = vec2d(a, 0);
  t.mesh_.vertecies[1] = vec2d::create_unit_from_degrees(degrees) * b;

  t.update_vertecies();
  return t;
}

Triangle msce::Triangle::create_asa(double deg_A, double a, double deg_C)
{
  if (deg_A + deg_C >= 180.0)
    logger.log_warning("Created invalid triangle(using ASA method) with 2 "
                       "angles provided >=180.0.(A={}, C={})",
                       deg_A, deg_C);

  Triangle t;
  t.mesh_.vertecies[0] = vec2d(0, 0);
  t.mesh_.vertecies[2] = vec2d(a, 0);

  const double A_rad = deg_to_rad(deg_A);
  const double C_rad = deg_to_rad(deg_C);
  const double B_rad = deg_to_rad(180 - deg_A - deg_C);

  const double ratio = a / sin(B_rad);
  const double ab_len = sin(C_rad) * ratio;

  t.mesh_.vertecies[1] = vec2d::create_unit_from_radians(A_rad) * ab_len;

  t.update_vertecies();
  return t;
}

const IMesh &msce::Triangle::get_mesh_ref() const { return mesh_; }

double msce::Triangle::get_perimetr() const
{

  return mesh_.vertecies[0].distance_to(mesh_.vertecies[1]) +
         mesh_.vertecies[1].distance_to(mesh_.vertecies[2]) +
         mesh_.vertecies[2].distance_to(mesh_.vertecies[1]);
}

double msce::Triangle::get_area() const
{
  return std::abs(this->mesh_.vertecies[2].cross(this->mesh_.vertecies[1])) / 2;
}

double msce::Triangle::get_h() const
{
  double maxy = std::max(
      {mesh_.vertecies[0].y, mesh_.vertecies[1].y, mesh_.vertecies[2].y});
  return maxy;
}
double msce::Triangle::get_w() const
{
  double maxx = std::max(
      {mesh_.vertecies[0].x, mesh_.vertecies[1].x, mesh_.vertecies[2].x});

  return maxx;
}

void msce::Rectangle::update_vertecies() noexcept
{
  mesh_.vertecies[0] = vec2d(0, 0);
  mesh_.vertecies[1] = vec2d(a_, 0);
  mesh_.vertecies[2] = vec2d(a_, b_);
  mesh_.vertecies[3] = vec2d(0, b_);
}

msce::Rectangle::Rectangle() {}

msce::Rectangle::Rectangle(double a, double b)
{
  a_ = a;
  b_ = b;

  update_vertecies();
}

const IMesh &msce::Rectangle::get_mesh_ref() const { return mesh_; }

double msce::Rectangle::get_perimetr() const { return 2 * (a_ + b_); }

double msce::Rectangle::get_area() const { return a_ * b_; }

double msce::Rectangle::get_w() const { return a_; }

double msce::Rectangle::get_h() const { return b_; }

double msce::Rectangle::a() const noexcept { return this->a_; }

void msce::Rectangle::a(double a) noexcept
{
  this->a_ = a;
  update_vertecies();
}

double msce::Rectangle::b() const noexcept { return this->b_; }

void msce::Rectangle::b(double b) noexcept
{
  this->b_ = b;
  update_vertecies();
}

msce::BaseShape2D::BaseShape2D() {}

void msce::Circle::update_vertecies() noexcept
{
  static const std::array<vec2d, MSCE_CIRCLE_VERTICES_COUNT> normal_verts = []()
  {
    constexpr const double rads_per_vertex_step =
        (2 * M_PI) / MSCE_CIRCLE_VERTICES_COUNT;
    std::array<vec2d, MSCE_CIRCLE_VERTICES_COUNT> res;
    for (int i = 0; i < MSCE_CIRCLE_VERTICES_COUNT; ++i)
    {
      double current_angle = i * rads_per_vertex_step;
      res[i] = vec2d(sin(current_angle), cos(current_angle));
    }

    return res;
  }();

  for (int i = 0; i < MSCE_CIRCLE_VERTICES_COUNT; i++)
  {
    mesh_.vertecies[i] = normal_verts[i] * r_ + r_;
  }
}

msce::Circle::Circle() {}

msce::Circle::Circle(double r)
{
  r_ = r;
  update_vertecies();
}

const IMesh &msce::Circle::get_mesh_ref() const { return mesh_; }

double msce::Circle::get_perimetr() const
{
  constexpr double two_pi = 2 * M_PI;
  return two_pi * r_;
}

double msce::Circle::get_area() const { return M_PI * r_ * r_; }

double msce::Circle::get_w() const { return r_ * 2; }

double msce::Circle::get_h() const { return r_ * 2; }

void msce::Circle::radius(double r) { r_ = r; }

double msce::Circle::radius() const noexcept { return r_; }
