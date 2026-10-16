#ifndef MSCE_SHAPE_H_
#define MSCE_SHAPE_H_

#define MSCE_CIRCLE_VERTICES_COUNT 32

#include <MSCE/msce_macros.h>
#include <MSCE/Types/vector.h>
#include <MSCE/reflection.h>
#include <variant>
#include "mesh.hpp"
#include <vector>
namespace msce
{
class Shape2D;

/** @brief Class representation of a triangle in 2D space.
 * @note Factory methods are aproximations, because they use unperfect
 * trygonometry.
 */
class Triangle
{
private:
  MSCE_REFLECTION_ALLOW_PRIVATE(Triangle);

  Mesh<3, 3> mesh_ =
      Mesh<3, 3>({vec2d(0, 0), vec2d(300, 0), vec2d(150, 300)},
                 {vec2d(0, 0), vec2d(1, 0), vec2d(0.5, 1)}, {0, 1, 2});

public:
  /// @brief Default constructor.
  Triangle();

  /**
   * @brief Creates triangle using SideSideSide method.
   * @param a Length of side AC
   * @param b Length of side AB
   * @param c Length of side BC
   * @returns Triangle with given sides.
   */
  static Triangle create_sss(double a, double b, double c);

  /**
   * @brief Creates triangle using SideAngleSide method.
   * @param a Lengrh of side AC.
   * @param b Length of side AB
   * @param degrees Angle between a and b in degrees.
   * @returns Triangle with given data.
   */
  static Triangle create_sas(double a, double degrees, double b);

  /**
   * @brief Creates triangle using AngleSideAngle method.
   * @param deg1 "Left" angle of side AC.
   * @param side Length if side AC.
   * @param deg2 "Right" angle of side AC.
   * @returns Triangle with given data.
   */
  static Triangle create_asa(double deg1, double side, double deg2);
  const IMesh &get_mesh_ref() const;
  double get_perimetr() const;
  double get_area() const;
  double get_h() const;
  double get_w() const;
  std::vector<vec2d> get_vertecies();

  operator Shape2D() const;

  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS()
};

/**
 * @brief Class representation of a rectangle in 2D space.
 */
class Rectangle
{
private:
  MSCE_REFLECTION_ALLOW_PRIVATE(Rectangle);
  /// @brief Width of the rectangle.
  double a_ = 300;

  /// @brief Height of the rectangle.
  double b_ = 300;

  inline static msce::Mesh<4, 6> mesh = msce::Mesh<4, 6>(
      {vec2d(0, 0), vec2d(1, 0), vec2d(1, 1), vec2d(0, 1)},
      {vec2d(0, 0), vec2d(1, 0), vec2d(1, 1), vec2d(0, 1)}, {0, 1, 2, 0, 2, 3});

public:
  /// @brief Default constructor
  Rectangle();
  /**
   * @brief Parameter constructor.
   * @param a The width of the rectangle.
   * @param b The height of the rectangle.
   */
  Rectangle(double a, double b);

  const IMesh &get_mesh_ref() const;

  double get_perimetr() const;
  double get_area() const;
  double get_w() const;
  double get_h() const;
  double get_diagonal() const;
  std::vector<vec2d> get_vertecies();

  /// @brief Getter for a_
  /// @return copy of a_
  double a() const noexcept;
  /// @brief Setter for a_
  /// @param a New value.
  void a(double) noexcept;

  /// @brief Getter for b_
  /// @return copy of b_
  double b() const noexcept;
  /// @brief Setter for b_
  /// @param a New value.
  void b(double) noexcept;

  operator Shape2D() const;

  friend class ::cereal::access;
  template <class Archive> void save(Archive &ar) const
  {
    ar(CEREAL_NVP(a_), CEREAL_NVP(b_));
  }
  template <class Archive> void load(Archive &ar)
  {
    ar(CEREAL_NVP(a_), CEREAL_NVP(b_));
  }
};

/**
 * @brief Class representation of a circle in 2D space.
 */
class Circle
{
private:
  MSCE_REFLECTION_ALLOW_PRIVATE(Circle);
  /// @brief Circle's radius.
  double r_ = 300;

  inline static Mesh<MSCE_CIRCLE_VERTICES_COUNT,
                     (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3>
      mesh = Mesh<MSCE_CIRCLE_VERTICES_COUNT,
                  (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3>(
          []() -> std::array<vec2d, MSCE_CIRCLE_VERTICES_COUNT>
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
          }(),
          []() -> std::array<vec2d, MSCE_CIRCLE_VERTICES_COUNT>
          {
            constexpr const double rads_per_vertex_step =
                (2 * M_PI) / MSCE_CIRCLE_VERTICES_COUNT;
            std::array<vec2d, MSCE_CIRCLE_VERTICES_COUNT> res;
            for (int i = 0; i < MSCE_CIRCLE_VERTICES_COUNT; ++i)
            {
              double current_angle = i * rads_per_vertex_step;
              res[i] =
                  vec2d(sin(current_angle), cos(current_angle)) * 0.5 + 0.5;
            }

            return res;
          }(),
          []() -> std::array<uint32_t, (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3>
          {
            std::array<uint32_t, (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3> res;
            for (uint32_t i = 1; i + 1 < MSCE_CIRCLE_VERTICES_COUNT; ++i)
            {
              res[(i - 1) * 3] = 0;
              res[(i - 1) * 3 + 1] = i;
              res[(i - 1) * 3 + 2] = i + 1;
            }
            return res;
          }());

public:
  /// @brief Default constructor.
  Circle();
  /**
   * @brief Param constructor.
   * @param r Radius of the circle.
   */
  Circle(double r);
  const IMesh &get_mesh_ref() const;

  double get_perimetr() const;
  double get_area() const;
  double get_w() const;
  double get_h() const;
  std::vector<vec2d> get_vertecies();

  /// @brief Setter for r_
  /// @param r New value.
  void radius(double);
  /// @brief Getter for r_
  /// @return copy of r_
  double radius() const noexcept;
  operator Shape2D() const;

  friend class ::cereal::access;
  template <class Archive> void save(Archive &ar) const { ar(CEREAL_NVP(r_)); }
  template <class Archive> void load(Archive &ar) { ar(CEREAL_NVP(r_)); }
};

using ShapeVariant = std::variant<Triangle, Rectangle, Circle>;
using Vec2dArray32 = std::array<vec2d, MSCE_CIRCLE_VERTICES_COUNT>;
using U32Array90 = std::array<uint32_t, (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3>;
using CircleMesh =
    Mesh<MSCE_CIRCLE_VERTICES_COUNT, (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3>;

struct Shape2D
{
  friend class ::cereal::access;
  ShapeVariant shape;

  /**
   * @returns All the vertecies of this shape.
   */
  const IMesh &get_mesh_ref() const
  {
    return std::visit([](const auto &s) -> const IMesh &
                      { return s.get_mesh_ref(); }, shape);
  }
  /**
   * @returns Calculated perimetr of this shape.
   */
  double get_perimetr() const
  {
    return std::visit([](const auto &s) -> double { return s.get_perimetr(); },
                      shape);
  }
  /**
   * @returns Calculated area of this shape.
   */
  double get_area() const
  {
    return std::visit([](const auto &s) -> double { return s.get_area(); },
                      shape);
  }
  /**
   * @returns Calculated height of rectangle containing this shape.
   */
  double get_h() const
  {
    return std::visit([](const auto &s) -> double { return s.get_h(); }, shape);
  }
  /**
   * @returns Calculated width of rectangle containing this shape.
   */
  double get_w() const
  {
    return std::visit([](const auto &s) -> double { return s.get_w(); }, shape);
  }

  std::vector<vec2d> get_vertecies()
  {
    return std::visit([](auto &s) -> std::vector<vec2d>
                      { return s.get_vertecies(); }, shape);
  };

  friend class ::cereal::access;
  template <class Archive> void save(Archive &ar) const
  {
    ar(CEREAL_NVP(shape));
  }
  template <class Archive> void load(Archive &ar) { ar(CEREAL_NVP(shape)); }

  Shape2D &operator=(Triangle s)
  {
    shape = std::move(s);
    return *this;
  }
  Shape2D &operator=(Circle s)
  {
    shape = std::move(s);
    return *this;
  }
  Shape2D &operator=(Rectangle s)
  {
    shape = std::move(s);
    return *this;
  }
};

} // namespace msce

MSCE_REFLECT_CLASS(msce::Triangle, mesh_)
MSCE_REFLECT_CLASS(msce::Rectangle, a_, b_)
MSCE_REFLECT_CLASS(msce::Circle, r_)
MSCE_REFLECT_CLASS(msce::ShapeVariant)
MSCE_REFLECT_CLASS(msce::Shape2D, shape)
MSCE_REFLECT_CLASS(msce::Vec2dArray32)
MSCE_REFLECT_CLASS(msce::U32Array90)
MSCE_REFLECT_CLASS(msce::CircleMesh, vertecies, texture_uv, indecies)
#endif