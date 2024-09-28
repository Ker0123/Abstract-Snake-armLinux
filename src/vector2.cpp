#include "vector2.h"
#include <cmath>

template <typename T>
Vector2<T>::Vector2(T x, T y)
{
    this->x = x;
    this->y = y;
}

template <typename T>
Vector2<T>::Vector2(int type, float length, float angle)
{
    this->x = length * cos(angle);
    this->y = length * sin(angle);
}


template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2 &other) const
{
    return Vector2(this->x + other.x, this->y + other.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2 &other) const
{
    return Vector2(this->x - other.x, this->y - other.y);
}

template <typename T>
Vector2<T> Vector2<T>::operator*(const T &scalar) const
{
    return Vector2(this->x * scalar, this->y * scalar);
}

template <typename T>
Vector2<T> Vector2<T>::operator/(const T &scalar) const
{
    return Vector2(this->x / scalar, this->y / scalar);
}

template <typename T>
Vector2<T> &Vector2<T>::operator+=(const Vector2 &other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator-=(const Vector2 &other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator*=(const T &scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

template <typename T>
Vector2<T> &Vector2<T>::operator/=(const T &scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

template <typename T>
bool Vector2<T>::operator==(const Vector2 &other) const
{
    return this->x == other.x && this->y == other.y;
}

template <typename T>
bool Vector2<T>::operator!=(const Vector2 &other) const
{
    return this->x != other.x || this->y != other.y;
}

template <typename T>
Vector2<T> &Vector2<T>::normalize()
{

    float length = this->length();
    if (length != 0)
    {
        this->x /= length;
        this->y /= length;
    }
    return *this;
}

template <typename T>
T Vector2<T>::length() const
{
    return sqrt(this->x * this->x + this->y * this->y);
}

template <typename T>
T Vector2<T>::angle() const
{
    return atan2(this->y, this->x);
}

template <typename U>
ostream &operator<<(ostream &os, const Vector2<U> &v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

template class Vector2<int>;
template ostream &operator<< (ostream &os, const Vector2<int> &v);

template class Vector2<float>;
template ostream &operator<< (ostream &os, const Vector2<float> &v);

template class Vector2<double>;
template ostream &operator<< (ostream &os, const Vector2<double> &v);