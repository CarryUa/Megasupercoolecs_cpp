#ifndef _MSCE_VECTORS_H_
#define _MSCE_VECTORS_H_

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
    Vector2D operator+(const VecCT &scalar);

    Vector2D &operator-=(const VecCT &scalar);
    Vector2D operator-(const VecCT &scalar);

    Vector2D &operator*=(const VecCT &scalar);
    Vector2D operator*(const VecCT &scalar);

    Vector2D &operator/=(const VecCT &scalar);
    Vector2D operator/(const VecCT &scalar);

    bool operator==(const VecCT &scalar) const;

    // Vector functions

    bool operator==(const Vector2D &other) const;

    VecCT Dot(const Vector2D &other);
};

#endif // _MSCE_VECTORS_H
