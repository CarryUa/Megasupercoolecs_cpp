#include "vector.h"

template <typename VecCT>
Vector2D<VecCT>::Vector2D(VecCT x, VecCT y)
{
    this->x = x;
    this->y = y;
}

template <typename VecCT>
Vector2D<VecCT>::Vector2D(const Vector2D &other)
{
    this->x = other.x;
    this->y = other.y;
}

#pragma region Scalar Operations

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator+=(const VecCT &scalar)
{
    this->x += scalar;
    this->y += scalar;
    return *this;
}

template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator+(const VecCT &scalar)
{
    return Vector2D(this->x + scalar, this->y + scalar);
}

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator-=(const VecCT &scalar)
{
    this->x -= scalar;
    this->y -= scalar;
    return *this;
}

template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator-(const VecCT &scalar)
{
    return Vector2D(this->x - scalar, this->y - scalar);
}

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator*=(const VecCT &scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}
template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator*(const VecCT &scalar)
{
    return Vector2D<VecCT>(this->x * scalar, this->y * scalar);
}

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator/=(const VecCT &scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}
template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator/(const VecCT &scalar)
{
    return Vector2D<VecCT>(this->x / scalar, this->y / scalar);
}

template <typename VecCT>
bool Vector2D<VecCT>::operator==(const VecCT &scalar) const
{
    return (this->x == scalar && this->y == scalar);
}

#pragma endregion

#pragma region Vector Operations

template <typename VecCT>
bool Vector2D<VecCT>::operator==(const Vector2D &other) const
{
    return (this->x == other.x && this->y == other.y);
}

#pragma endregion

template class Vector2D<int>;
template class Vector2D<unsigned int>;
template class Vector2D<float>;
template class Vector2D<char>;
template class Vector2D<unsigned char>;
template class Vector2D<double>;
template class Vector2D<long>;
template class Vector2D<unsigned long>;