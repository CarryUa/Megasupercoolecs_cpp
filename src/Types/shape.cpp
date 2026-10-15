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

std::vector<vec2d> msce::Triangle::get_vertecies()
{
  return std::vector<vec2d>();
}

msce::Triangle::operator Shape2D() const { return Shape2D{.shape = *this}; }

msce::Rectangle::Rectangle() {}

msce::Rectangle::Rectangle(double a, double b)
{
  a_ = a;
  b_ = b;
}

const IMesh &msce::Rectangle::get_mesh_ref() const { return mesh; }

double msce::Rectangle::get_perimetr() const { return 2 * (a_ + b_); }

double msce::Rectangle::get_area() const { return a_ * b_; }

double msce::Rectangle::get_w() const { return a_; }

double msce::Rectangle::get_h() const { return b_; }

double msce::Rectangle::get_diagonal() const
{
  return std::sqrt(a_ * a_ + b_ * b_);
}

std::vector<vec2d> msce::Rectangle::get_vertecies()
{
  std::vector<vec2d> r;
  r.reserve(4);
  r.push_back(vec2d(0, 0));
  r.push_back(vec2d(a_, 0));
  r.push_back(vec2d(a_, b_));
  r.push_back(vec2d(0, b_));

  return r;
}

double msce::Rectangle::a() const noexcept { return this->a_; }

void msce::Rectangle::a(double a) noexcept { this->a_ = a; }

double msce::Rectangle::b() const noexcept { return this->b_; }

void msce::Rectangle::b(double b) noexcept { this->b_ = b; }

msce::Rectangle::operator Shape2D() const { return Shape2D{.shape = *this}; }

msce::Circle::Circle() {}

msce::Circle::Circle(double r) { r_ = r; }

const IMesh &msce::Circle::get_mesh_ref() const { return mesh; }

double msce::Circle::get_perimetr() const
{
  constexpr double two_pi = 2 * M_PI;
  return two_pi * r_;
}

double msce::Circle::get_area() const { return M_PI * r_ * r_; }

double msce::Circle::get_w() const { return r_ * 2; }

double msce::Circle::get_h() const { return r_ * 2; }

std::vector<vec2d> msce::Circle::get_vertecies()
{
  std::vector<vec2d> r;
  r.reserve(mesh.vertex_count());
  for (vec2d v : mesh.vertecies)
    r.push_back(v * r_ + r_);

  return r;
}

void msce::Circle::radius(double r) { r_ = r; }

double msce::Circle::radius() const noexcept { return r_; }

msce::Circle::operator Shape2D() const { return Shape2D{.shape = *this}; }
