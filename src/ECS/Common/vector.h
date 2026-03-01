#ifndef _MSCE_VECTORS_H_
#define _MSCE_VECTORS_H_

namespace msce
{
    // 2D Vector structure with basic operations.
    template <typename VecCT>
    struct Vector2D
    {
        VecCT x;
        VecCT y;
        Vector2D(VecCT x = 0, VecCT y = 0);
        Vector2D(const Vector2D &Other);

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

        /// @brief Calculates dot(aka scalar) product of 2 vectors as if they were normalized
        /// @param other The second vector.
        /// @return The dot(aka scalar) product of given type.
        double normalized_dot(const Vector2D &other) const;

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
}

#endif // _MSCE_VECTORS_H
