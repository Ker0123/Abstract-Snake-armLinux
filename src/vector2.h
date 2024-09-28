#pragma once

#include <iostream>

using namespace std;

#define X_AND_Y 0
#define L_AND_A 1

template <typename T>
class Vector2
{
public:
    T x;
    T y;

    // 构造函数(不同构造方法)
    Vector2(T x = 0, T y = 0);
    Vector2(int type, float length, float angle);

    // 操作符重载
    Vector2 operator+(const Vector2 &other) const;
    Vector2 operator-(const Vector2 &other) const;
    Vector2 operator*(const T &scalar) const;
    Vector2 operator/(const T &scalar) const;
    Vector2 &operator+=(const Vector2 &other);
    Vector2 &operator-=(const Vector2 &other);
    Vector2 &operator*=(const T &scalar);
    Vector2 &operator/=(const T &scalar);
    bool operator==(const Vector2 &other) const;
    bool operator!=(const Vector2 &other) const;

    template <typename U>
    friend ostream &operator<<(ostream &os, const Vector2<U> &vec);

    // 获取简单推算出的属性
    Vector2 &normalize();
    T length() const;
    T angle() const;
};
