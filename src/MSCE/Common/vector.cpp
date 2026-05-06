#include "vector.h"
#include <cmath>
#include <MSCE/logger.h>

using namespace msce;
#pragma region Vector2D

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
    auto len = this->length();
    return static_cast<Vector2D<double>>(*this) / len;
}

template <typename VecCT>
double Vector2D<VecCT>::normalized_dot(const Vector2D &other) const
{
    Vector2D<double> this_normalized = this->normalized();
    Vector2D<double> other_normalized = other.normalized();

    return this_normalized.dot(other_normalized);
}
template <typename VecCT>
template <typename NewVecCT>
Vector2D<VecCT>::operator Vector2D<NewVecCT>() const
{
    return Vector2D<NewVecCT>(
        static_cast<NewVecCT>(this->x),
        static_cast<NewVecCT>(this->y));
}

#pragma endregion
#pragma endregion // Vector2D

#pragma region Vector3D
template <typename VecCT>
msce::Vector3D<VecCT>::Vector3D(VecCT x, VecCT y, VecCT z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator+=(const VecCT &scalar)
{
    this->x += scalar;
    this->y += scalar;
    this->z += scalar;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator+(const VecCT &scalar) const
{
    return Vector3D<VecCT>(this->x + scalar,
                           this->y + scalar,
                           this->z + scalar);
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator-=(const VecCT &scalar)
{
    this->x -= scalar;
    this->y -= scalar;
    this->z -= scalar;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator-(const VecCT &scalar) const
{
    return Vector3D<VecCT>(this->x - scalar,
                           this->y - scalar,
                           this->z - scalar);
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator*=(const VecCT &scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator*(const VecCT &scalar) const
{
    return Vector3D<VecCT>(this->x * scalar,
                           this->y * scalar,
                           this->z * scalar);
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator/=(const VecCT &scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator/(const VecCT &scalar) const
{
    return Vector3D<VecCT>(this->x / scalar,
                           this->y / scalar,
                           this->z / scalar);
}

template <typename VecCT>
double msce::Vector3D<VecCT>::length() const
{
    return sqrt(static_cast<double>(this->x) * static_cast<double>(this->x) +
                static_cast<double>(this->y) * static_cast<double>(this->y) +
                static_cast<double>(this->z) * static_cast<double>(this->z));
}

template <typename VecCT>
double msce::Vector3D<VecCT>::dot(const Vector3D<VecCT> &other) const
{
    return static_cast<double>(this->x) * static_cast<double>(other.x) +
           static_cast<double>(this->y) * static_cast<double>(other.y) +
           static_cast<double>(this->z) * static_cast<double>(other.z);
}

template <typename VecCT>
double msce::Vector3D<VecCT>::normalized_dot(const Vector3D<VecCT> &other) const
{
    const Vector3D<double> other_normalized = other.normalized();
    const Vector3D<double> this_normalized = this->normalized();
    return this_normalized.dot(other_normalized);
}

template <typename VecCT>
bool msce::Vector3D<VecCT>::operator==(const VecCT &scalar) const
{
    return this->x == scalar &&
           this->y == scalar &&
           this->z == scalar;
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator+=(const Vector3D<VecCT> &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator+(const Vector3D<VecCT> &other) const
{
    return Vector3D<VecCT>(this->x + other.x,
                           this->y + other.y,
                           this->z + other.z);
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator-=(const Vector3D<VecCT> &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator-(const Vector3D<VecCT> &other) const
{
    return Vector3D<VecCT>(this->x - other.x,
                           this->y - other.y,
                           this->z - other.z);
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator*=(const Vector3D<VecCT> &other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator*(const Vector3D<VecCT> &other) const
{
    return Vector3D<VecCT>(this->x * other.x,
                           this->y * other.y,
                           this->z * other.z);
}

template <typename VecCT>
Vector3D<VecCT> &msce::Vector3D<VecCT>::operator/=(const Vector3D<VecCT> &other)
{
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}

template <typename VecCT>
Vector3D<VecCT> msce::Vector3D<VecCT>::operator/(const Vector3D<VecCT> &other) const
{
    return Vector3D<VecCT>(this->x / other.x,
                           this->y / other.y,
                           this->z / other.z);
}

template <typename VecCT>
Vector3D<double> msce::Vector3D<VecCT>::normalized() const
{
    auto len = this->length();
    return static_cast<Vector3D<double>>(*this) / len;
}

template <typename VecCT>
bool msce::Vector3D<VecCT>::operator==(const Vector3D<VecCT> &other) const
{
    return this->x == other.x &&
           this->y == other.y &&
           this->z == other.z;
}

template <typename VecCT>
template <typename NewVecCT>
Vector3D<VecCT>::operator Vector3D<NewVecCT>() const
{
    return Vector3D<NewVecCT>(
        static_cast<NewVecCT>(this->x),
        static_cast<NewVecCT>(this->y),
        static_cast<NewVecCT>(this->z));
}

#pragma endregion

template class msce::Vector2D<int>;
template class msce::Vector2D<unsigned int>;
template class msce::Vector2D<float>;
template class msce::Vector2D<char>;
template class msce::Vector2D<unsigned char>;
template class msce::Vector2D<double>;
template class msce::Vector2D<long>;
template class msce::Vector2D<unsigned long>;

template class msce::Vector3D<int>;
template class msce::Vector3D<unsigned int>;
template class msce::Vector3D<float>;
template class msce::Vector3D<char>;
template class msce::Vector3D<unsigned char>;
template class msce::Vector3D<double>;
template class msce::Vector3D<long>;
template class msce::Vector3D<unsigned long>;