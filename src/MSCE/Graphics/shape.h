#ifndef MSCE_SHAPE_H_
#define MSCE_SHAPE_H_

#define MSCE_CIRCLE_VERTICES_COUNT 32

#include <MSCE/msce_macros.h>
#include <MSCE/Types/vector.h>
#include <vector>
namespace msce
{
    using Vertex = vec2d;
    using VertexList = std::vector<Vertex>;

    enum class TransformMethod : unsigned char
    {
        /// @brief move all vertecies on the X axis.
        translate_x,
        /// @brief move all vertecies on the Y axis.
        translate_y,
        /// @brief move all vertecies on all axises by equal distance.
        translate_xy,
        /// @brief set X component of scale.
        scale_x,
        /// @brief set Y component of scale.
        scale_y,
        /// @brief set bouth components of scale.
        scale_xy,
        /// @brief rotate object on X axis. IN DEGREES
        rotate_x,
        /// @brief rotate object on Y axis. IN DEGREES
        rotate_y,
        /// @brief rotate object on all axises by equal value. IN DEGREES
        rotate_xy,
    };

    class IShape2D
    {
    private:
    public:
        friend class ::cereal::access;
        template <class Archive>
        void save(Archive &ar) const {}
        template <class Archive>
        void load(Archive &ar) {}

    public:
        virtual ~IShape2D() = default;
        virtual VertexList get_vertecies() const = 0;
        virtual double get_perimetr() const = 0;
        virtual double get_area() const = 0;
        virtual double get_h() const = 0;
        virtual double get_w() const = 0;
    };

    class BaseShape2D : public IShape2D
    {
    public:
        BaseShape2D();
    };

    class Triangle : public BaseShape2D
    {
    private:
        vec2d a_ = vec2d(0, 0), b_ = vec2d(300, 0), c_ = vec2d(150, 300);

    public:
        Triangle();

        static Triangle create_sss(double a, double b, double c);
        static Triangle create_sas(double a, double degrees, double b);
        static Triangle create_asa(double deg1, double side, double deg2);

        virtual VertexList get_vertecies() const override;
        virtual double get_perimetr() const override;
        virtual double get_area() const override;
        virtual double get_h() const override;
        virtual double get_w() const override;

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, a_, b_, c_)
    };

    // class Square : public BaseShape2D
    // {
    // private:
    //     double a_ = 0;

    // public:
    //     Square();

    //     virtual VertexList get_vertecies() const override;
    //     virtual double get_perimetr() const override;
    //     virtual double get_area() const override;

    //     MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, a_)
    // };
    class Rectangle : public BaseShape2D
    {
    private:
        double a_ = 300, b_ = 300;

    public:
        Rectangle();
        Rectangle(double a, double b);

        virtual VertexList get_vertecies() const override;
        virtual double get_perimetr() const override;
        virtual double get_area() const override;
        virtual double get_w() const override;
        virtual double get_h() const override;
        double a() const noexcept;
        void a(double) noexcept;

        double b() const noexcept;
        void b(double) noexcept;

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, a_, b_)
    };

    class Circle : public BaseShape2D
    {
    private:
        double r_ = 300;

    public:
        Circle();
        Circle(double r);
        virtual VertexList get_vertecies() const override;
        virtual double get_perimetr() const override;
        virtual double get_area() const override;
        virtual double get_w() const override;
        virtual double get_h() const override;

        void radius(double);
        double radius() const noexcept;

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, r_)
    };

}
CEREAL_REGISTER_TYPE(msce::IShape2D)
CEREAL_REGISTER_TYPE(msce::BaseShape2D)
CEREAL_REGISTER_TYPE(msce::Triangle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Triangle, msce::BaseShape2D)
// CEREAL_REGISTER_TYPE(msce::Square)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Square, msce::BaseShape2D)
CEREAL_REGISTER_TYPE(msce::Rectangle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Rectangle, msce::BaseShape2D)
CEREAL_REGISTER_TYPE(msce::Circle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Circle, msce::BaseShape2D)

#endif