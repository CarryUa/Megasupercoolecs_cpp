#ifndef _MSCE_VECTORS_H_
#define _MSCE_VECTORS_H_
#include <MSCE/msce_macros.h>

namespace msce
{
    /// @brief 2D Vector structure with basic operations.
    template <typename VecCT>
    struct Vector2D
    {
    private:
        MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(x, y)
    public:
        VecCT x;
        VecCT y;
        Vector2D(VecCT x = 0, VecCT y = 0);
        Vector2D(const Vector2D &Other);

        static Vector2D<double> create_unit_from_radians(double);
        static Vector2D<double> create_unit_from_degrees(double);

        // Scalar operations
        Vector2D &operator+=(const VecCT &scalar);
        Vector2D operator+(const VecCT &scalar) const;

        Vector2D &operator-=(const VecCT &scalar);
        Vector2D operator-(const VecCT &scalar) const;

        Vector2D &operator*=(const VecCT &scalar);
        Vector2D operator*(const VecCT &scalar) const;

        Vector2D &operator/=(const VecCT &scalar);
        Vector2D operator/(const VecCT &scalar) const;

        /// @brief Calculates the length of a vector.
        /// @return Length of the vector of given type.
        double length() const;

        /// @brief Calculates the dot(aka scalar) product of 2 vectors.
        /// @param other The second vector.
        /// @return The dot(aka scalar) product of given type.
        double dot(const Vector2D &other) const;
        double cross(const Vector2D &other) const;

        /// @brief Calculates dot(aka scalar) product of 2 vectors as if they were normalized
        /// @param other The second vector.
        /// @return The dot(aka scalar) product of given type.
        double normalized_dot(const Vector2D &other) const;

        double get_angle_between_radians(const Vector2D<VecCT> &other) const noexcept;
        double get_angle_between_degrees(const Vector2D<VecCT> &other) const noexcept;

        VecCT distance_to(const Vector2D<VecCT> &) const noexcept;

        bool operator==(const VecCT &scalar) const;

        // Vector functions

        Vector2D &operator+=(const Vector2D &other);
        Vector2D operator+(const Vector2D &other) const;

        Vector2D &operator-=(const Vector2D &other);
        Vector2D operator-(const Vector2D &other) const;

        Vector2D &operator*=(const Vector2D &other);
        Vector2D operator*(const Vector2D &other) const;

        Vector2D &operator/=(const Vector2D &other);
        Vector2D operator/(const Vector2D &other) const;

        /// @brief Calculates the normalized vector that points in a same direction as on current one.
        /// @return Normalized vector.
        Vector2D<double> normalized() const;

        bool operator==(const Vector2D &other) const;

        // Casts
        template <typename NewVecCT>
        operator Vector2D<NewVecCT>() const;
    };

    /// @brief 3D Vector structure with basic operations.
    template <typename VecCT>
    struct Vector3D
    {
        VecCT x;
        VecCT y;
        VecCT z;
        Vector3D(VecCT x = 0, VecCT y = 0, VecCT z = 0);
        // Vector3D(const Vector3D<VecCT>&Other);

        // Scalar operations
        Vector3D<VecCT> &operator+=(const VecCT &scalar);
        Vector3D<VecCT> operator+(const VecCT &scalar) const;

        Vector3D<VecCT> &operator-=(const VecCT &scalar);
        Vector3D<VecCT> operator-(const VecCT &scalar) const;

        Vector3D<VecCT> &operator*=(const VecCT &scalar);
        Vector3D<VecCT> operator*(const VecCT &scalar) const;

        Vector3D<VecCT> &operator/=(const VecCT &scalar);
        Vector3D<VecCT> operator/(const VecCT &scalar) const;

        /// @brief Calculates the length of a vector.
        /// @return Length of the vector of given type.
        double length() const;

        /// @brief Calculates the dot(aka scalar) product of 2 vectors.
        /// @param other The second vector.
        /// @return The dot(aka scalar) product of given type.
        double dot(const Vector3D<VecCT> &other) const;

        /// @brief Calculates dot(aka scalar) product of 2 vectors as if they were normalized
        /// @param other The second vector.
        /// @return The dot(aka scalar) product of given type.
        double normalized_dot(const Vector3D<VecCT> &other) const;

        bool operator==(const VecCT &scalar) const;

        // Vector functions

        Vector3D<VecCT> &operator+=(const Vector3D<VecCT> &other);
        Vector3D<VecCT> operator+(const Vector3D<VecCT> &other) const;

        Vector3D<VecCT> &operator-=(const Vector3D<VecCT> &other);
        Vector3D<VecCT> operator-(const Vector3D<VecCT> &other) const;

        Vector3D<VecCT> &operator*=(const Vector3D<VecCT> &other);
        Vector3D<VecCT> operator*(const Vector3D<VecCT> &other) const;

        Vector3D<VecCT> &operator/=(const Vector3D<VecCT> &other);
        Vector3D<VecCT> operator/(const Vector3D<VecCT> &other) const;

        /// @brief Calculates the normalized vector that points in a same direction as on current one.
        /// @return Normalized vector.
        Vector3D<double> normalized() const;

        bool operator==(const Vector3D<VecCT> &other) const;

        // Casts
        template <typename NewVecCT>
        operator Vector3D<NewVecCT>() const;

    private:
        MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(x, y, z)
    };

    using vec2i = Vector2D<int>;
    using vec2ui = Vector2D<unsigned int>;
    using vec2f = Vector2D<float>;
    using vec2c = Vector2D<char>;
    using vec2uc = Vector2D<unsigned char>;
    using vec2d = Vector2D<double>;
    using vec2i64 = Vector2D<long>;
    using vec2ui64 = Vector2D<unsigned long>;
    using vec2l = Vector2D<long>;
    using vec2ul = Vector2D<unsigned long>;

    using vec3i = Vector3D<int>;
    using vec3ui = Vector3D<unsigned int>;
    using vec3f = Vector3D<float>;
    using vec3c = Vector3D<char>;
    using vec3uc = Vector3D<unsigned char>;
    using vec3d = Vector3D<double>;
    using vec3i64 = Vector3D<long>;
    using vec3ui64 = Vector3D<unsigned long>;
    using vec3l = Vector3D<long>;
    using vec3ul = Vector3D<unsigned long>;
}

#endif // _MSCE_VECTORS_H
