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
        TRANSLATE_X,
        /// @brief move all vertecies on the Y axis.
        TRANSLATE_Y,
        /// @brief move all vertecies on all axises by equal distance.
        TRANSLATE_XY,
        /// @brief set X component of scale.
        SCALE_X,
        /// @brief set Y component of scale.
        SCALE_Y,
        /// @brief set bouth components of scale.
        SCALE_XY,
        /// @brief rotate object on X axis. IN DEGREES
        ROTATE_X,
        /// @brief rotate object on Y axis. IN DEGREES
        ROTATE_Y,
        /// @brief rotate object on all axises by equal value. IN DEGREES
        ROTATE_XY,
    };

    /**
     * @brief Shape interface for storing purposes. */
    class IShape2D
    {
    private:
    public:
        friend class ::cereal::access;
        /// @brief Cereal's save method. See cereal's docs for more info.
        template <class Archive>
        void save(Archive &ar) const {}
        /// @brief Cereal's load method. See cereal's docs for more info.
        template <class Archive>
        void load(Archive &ar) {}

    public:
        virtual ~IShape2D() = default;
        /**
         * @returns All the vertecies of this shape.
         */
        virtual VertexList get_vertecies() const = 0;
        /**
         * @returns Calculated perimetr of this shape.
         */
        virtual double get_perimetr() const = 0;
        /**
         * @returns Calculated area of this shape.
         */
        virtual double get_area() const = 0;
        /**
         * @returns Calculated height of rectangle containing this shape.
         */
        virtual double get_h() const = 0;
        /**
         * @returns Calculated width of rectangle containing this shape.
         */
        virtual double get_w() const = 0;
    };

    /** @brief Base type for all 2D shapes. */
    class BaseShape2D : public IShape2D
    {
    public:
        /// @brief Default constructor.
        BaseShape2D();
    };

    /** @brief Class representation of a triangle in 2D space.
     * @note Factory methods are aproximations, because they use unperfect trygonometry.
     */
    class Triangle : public BaseShape2D
    {
    private:
        vec2d a_ = vec2d(0, 0), b_ = vec2d(300, 0), c_ = vec2d(150, 300);

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

        virtual VertexList get_vertecies() const override;
        virtual double get_perimetr() const override;
        virtual double get_area() const override;
        virtual double get_h() const override;
        virtual double get_w() const override;

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, a_, b_, c_)
    };

    /**
     * @brief Class representation of a rectangle in 2D space.
     */
    class Rectangle : public BaseShape2D
    {
    private:
        /// @brief Width of the rectangle.
        double a_ = 300;

        /// @brief Height of the rectangle.
        double b_ = 300;

    public:
        /// @brief Default constructor
        Rectangle();
        /**
         * @brief Parameter constructor.
         * @param a The width of the rectangle.
         * @param b The height of the rectangle.
         */
        Rectangle(double a, double b);

        virtual VertexList get_vertecies() const override;
        virtual double get_perimetr() const override;
        virtual double get_area() const override;
        virtual double get_w() const override;
        virtual double get_h() const override;
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

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, a_, b_)
    };

    /**
     * @brief Class representation of a circle in 2D space.
     */
    class Circle : public BaseShape2D
    {
    private:
        /// @brief Circle's radius.
        double r_ = 300;

    public:
        /// @brief Default constructor.
        Circle();
        /**
         * @brief Param constructor.
         * @param r Radius of the circle.
         */
        Circle(double r);
        virtual VertexList get_vertecies() const override;
        virtual double get_perimetr() const override;
        virtual double get_area() const override;
        virtual double get_w() const override;
        virtual double get_h() const override;

        /// @brief Setter for r_
        /// @param r New value.
        void radius(double);
        /// @brief Getter for r_
        /// @return copy of r_
        double radius() const noexcept;

        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(BaseShape2D, r_)
    };

}
CEREAL_REGISTER_TYPE(msce::IShape2D)
CEREAL_REGISTER_TYPE(msce::BaseShape2D)
CEREAL_REGISTER_TYPE(msce::Triangle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Triangle, msce::BaseShape2D)
CEREAL_REGISTER_TYPE(msce::Rectangle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Rectangle, msce::BaseShape2D)
CEREAL_REGISTER_TYPE(msce::Circle)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::Circle, msce::BaseShape2D)

#endif