#include "vector.h"
#include <cmath>

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

template <typename VecCT>
template <typename NewVecCT>
Vector2D<VecCT>::operator Vector2D<NewVecCT>() const
{
    return Vector2D<NewVecCT>(
        static_cast<NewVecCT>(this->x),
        static_cast<NewVecCT>(this->y));
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
Vector2D<VecCT> Vector2D<VecCT>::operator+(const VecCT &scalar) const
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
Vector2D<VecCT> Vector2D<VecCT>::operator-(const VecCT &scalar) const
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
Vector2D<VecCT> Vector2D<VecCT>::operator*(const VecCT &scalar) const
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
Vector2D<VecCT> Vector2D<VecCT>::operator/(const VecCT &scalar) const
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
Vector2D<VecCT> &Vector2D<VecCT>::operator+=(const Vector2D &other)
{
    this->x += other.x;
    this->y += other.y;

    return *this;
}

template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator+(const Vector2D &other) const
{
    return Vector2D(this->x + other.x,
                    this->y + other.y);
}

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator-=(const Vector2D &other)
{
    this->x -= other.x;
    this->y -= other.y;

    return *this;
}

template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator-(const Vector2D &other) const
{
    return Vector2D(this->x - other.x,
                    this->y - other.y);
}

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator*=(const Vector2D &other)
{
    this->x *= other.x;
    this->y *= other.y;

    return *this;
}

template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator*(const Vector2D &other) const
{
    return Vector2D(this->x * other.x,
                    this->y * other.y);
}

template <typename VecCT>
Vector2D<VecCT> &Vector2D<VecCT>::operator/=(const Vector2D &other)
{
    this->x /= other.x;
    this->y /= other.y;

    return *this;
}

template <typename VecCT>
Vector2D<VecCT> Vector2D<VecCT>::operator/(const Vector2D &other) const
{
    return Vector2D(this->x / other.x,
                    this->y / other.y);
}

template <typename VecCT>
bool Vector2D<VecCT>::operator==(const Vector2D &other) const
{
    return (this->x == other.x && this->y == other.y);
}

#pragma endregion

#pragma region Other Functions

template <typename VecCT>
double Vector2D<VecCT>::length() const
{
    // Pitagorasian theorem
    return sqrt(static_cast<double>(this->x) * static_cast<double>(this->x) +
                static_cast<double>(this->y) * static_cast<double>(this->y));
}

template <typename VecCT>
double Vector2D<VecCT>::dot(const Vector2D &other) const
{
    return static_cast<double>(this->x) * static_cast<double>(other.x) +
           static_cast<double>(this->y) * static_cast<double>(other.y);
}

template <typename VecCT>
Vector2D<double> Vector2D<VecCT>::normalized() const
{
    return static_cast<Vector2D<double>>(*this) / this->length();
}

template <typename VecCT>
double Vector2D<VecCT>::normalized_dot(const Vector2D &other) const
{
    Vector2D<double> this_normalized = this->normalized();
    Vector2D<double> other_normalized = other.normalized();

    return this_normalized.dot(other_normalized);
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