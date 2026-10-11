#ifndef MSCE_SHAPE_H_
#define MSCE_SHAPE_H_

#define MSCE_CIRCLE_VERTICES_COUNT 32

#include <MSCE/msce_macros.h>
#include <MSCE/Types/vector.h>
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
  Mesh<3, 3> mesh_ =
      Mesh<3, 3>({vec2d(0, 0), vec2d(300, 0), vec2d(150, 300)},
                 {vec2d(0, 0), vec2d(1, 0), vec2d(0.5, 1)}, {0, 1, 2});

  void update_vertecies() noexcept;

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

  operator Shape2D() const;

  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(mesh_)
};

/**
 * @brief Class representation of a rectangle in 2D space.
 */
class Rectangle
{
private:
  /// @brief Width of the rectangle.
  double a_ = 300;

  /// @brief Height of the rectangle.
  double b_ = 300;

  Mesh<4, 6> mesh_ = Mesh<4, 6>(
      {vec2d(0, 0), vec2d(a_, 0), vec2d(a_, b_), vec2d(0, b_)},
      {vec2d(0, 0), vec2d(1, 0), vec2d(1, 1), vec2d(0, 1)}, {0, 1, 2, 0, 2, 3});

  void update_vertecies() noexcept;

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

  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(a_, b_)
};

/**
 * @brief Class representation of a circle in 2D space.
 */
class Circle
{
private:
  /// @brief Circle's radius.
  double r_ = 300;

  Mesh<MSCE_CIRCLE_VERTICES_COUNT, (MSCE_CIRCLE_VERTICES_COUNT - 2) * 3> mesh_;

  void update_vertecies() noexcept;

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

  /// @brief Setter for r_
  /// @param r New value.
  void radius(double);
  /// @brief Getter for r_
  /// @return copy of r_
  double radius() const noexcept;
  operator Shape2D() const;

  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(r_)
};

struct Shape2D
{
  friend class ::cereal::access;
  std::variant<Triangle, Rectangle, Circle> shape;

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
#endif