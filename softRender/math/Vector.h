#ifndef VECTOR_H
#define VECTOR_H
#include <assert.h>
#include <cmath>
#include <iostream>
namespace Maths {

//向量模板
template <int size>
struct Vector {
    Vector() = default;
    float& operator[](const int i) {
        assert(i >= 0 && i < size);
        return data[i];
    }
    float operator[](const int i) const {
        assert(i >= 0 && i < size);
        return data[i];
    }
    float norm2() const { return *this * *this; }
    float norm() const { return std::sqrt(norm2()); }
    float data[size] = {0};
};
//向量加法运算
template <int size>
Vector<size> operator+(const Vector<size>& lhs, const Vector<size>& rhs) {
    Vector<size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = lhs[i] + rhs[i];
    }
    return result;
}
//向量减法运算
template <int size>
Vector<size> operator-(const Vector<size>& lhs, const Vector<size>& rhs) {
    Vector<size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = lhs[i] - rhs[i];
    }
    return result;
}
//向量点乘运算
template <int size>
float operator*(const Vector<size>& lhs, const Vector<size>& rhs) {
    float result = 0;
    for (int i = 0; i < size; ++i) {
        result += lhs[i] * rhs[i];
    }
    return result;
}
//常数乘向量
template <int size>
Vector<size> operator*(const float lhs, const Vector<size>& rhs) {
    Vector<size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = lhs * rhs[i];
    }
    return result;
}
//向量乘常数
template <int size>
Vector<size> operator*(const Vector<size>& lhs, const float rhs) {
    Vector<size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = lhs[i] * rhs;
    }
    return result;
}
//向量除常数
template <int size>
Vector<size> operator/(const Vector<size>& lhs, const float rhs) {
    Vector<size> result;
    for (int i = 0; i < size; ++i) {
        result[i] = lhs[i] / rhs;
    }
    return result;
}
//
template <int size1, int size2>
Vector<size1> embeded(const Vector<size2>& v, const float fill = 1) {
    Vector<size1> result;
    for (int i = 0; i < size1; ++i) {
        result[i] = (i >= size2) ? fill : v[i];
    }
    return result;
}
// todo proj
//向量输出
template <int size>
std::ostream& operator<<(std::ostream& out, const Vector<size> v) {
    for (int i = 0; i < size; ++i) {
        std::cout << v[i] << " ";
    }
    return out;
}
//特化2维向量
template <>
struct Vector<2> {
    Vector() = default;
    Vector(float ix, float iy) {
        data[0] = ix;
        data[1] = iy;
    }
    float& operator[](const int i) {
        assert(i >= 0 && i < 2);
        return data[i];
    }
    float operator[](const int i) const {
        assert(i >= 0 && i < 2);
        return data[i];
    }
    float x() const { return data[0]; };
    float y() const { return data[1]; };
    float norm2() const { return *this * *this; }
    float norm() const { return std::sqrt(norm2()); }
    Vector& normalize() {
        *this = (*this) / norm();
        return *this;
    }
    float data[2] = {0};
};
//特化3维向量
template <>
struct Vector<3> {
    Vector() = default;
    Vector(float ix, float iy, float iz) {
        data[0] = ix;
        data[1] = iy;
        data[2] = iz;
    }
    float& operator[](const int i) {
        assert(i >= 0 && i < 3);
        return data[i];
    }
    float operator[](const int i) const {
        assert(i >= 0 && i < 3);
        return data[i];
    }
    float x() const { return data[0]; };
    float y() const { return data[1]; };
    float z() const { return data[2]; };
    float norm2() const { return *this * *this; }
    float norm() const { return std::sqrt(norm2()); }
    Vector& normalize() {
        *this = (*this) / norm();
        return *this;
    }
    float data[3] = {0};
};
//特化4维向量
template <>
struct Vector<4> {
    Vector() = default;
    Vector(float ix, float iy, float iz, float iw = 0.0f) {
        data[0] = ix;
        data[1] = iy;
        data[2] = iz;
        data[3] = iw;
    }
    float& operator[](const int i) {
        assert(i >= 0 && i < 4);
        return data[i];
    }
    float operator[](const int i) const {
        assert(i >= 0 && i < 4);
        return data[i];
    }
    float x() const { return data[0]; };
    float y() const { return data[1]; };
    float z() const { return data[2]; };
    float w() const { return data[3]; };
    float norm2() const { return *this * *this; }
    float norm() const { return std::sqrt(norm2()); }
    Vector& normalize() {
        *this = (*this) / norm();
        return *this;
    }
    float data[4] = {0};
};
typedef Vector<2> Vector2f;
typedef Vector<3> Vector3f;
typedef Vector<4> Vector4f;
//向量叉乘
Vector3f cross(const Vector3f& v1, const Vector3f& v2) {
    return Vector<3>{v1.y() * v2.z() - v1.z() * v2.y(),
                     v1.z() * v2.x() - v1.x() * v2.z(),
                     v1.x() * v2.y() - v1.y() * v2.x()};
}
}  // namespace Maths
#endif