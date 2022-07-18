#ifndef VECTOR_H
#define VECTOR_H
#include <assert.h>
#include <cmath>
#include <iostream>
namespace Maths {
    //列优先向量
    //向量模板
    template <int size>
    struct Vector {
        Vector() = default;
        float &operator[](const int i) {
            assert(i >= 0 && i < size);
            return data[i];
        }
        float operator[](const int i) const {
            assert(i >= 0 && i < size);
            return data[i];
        }
        float norm2() const {
            float result = 0.0f;
            for (int i = 0; i < size; ++i) {
                result += data[i] * data[i];
            }
            return result;
        }
        float norm() const {
            return std::sqrt(norm2());
        }
        float Dot(const Vector<size> &rhs) {
            float result = 0;
            for (int i = 0; i < size; ++i) {
                result += data[i] * rhs[i];
            }
            return result;
        }
        float Dot(const Vector<size> &rhs) const {
            float result = 0;
            for (int i = 0; i < size; ++i) {
                result += data[i] * rhs[i];
            }
            return result;
        }
        float data[size] = {0};
    };
    //向量加法运算
    template <int size>
    Vector<size> operator+(const Vector<size> &lhs, const Vector<size> &rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs[i] + rhs[i];
        }
        return result;
    }
        //向量加法运算
    template <int size>
    Vector<size> operator+(const Vector<size> &lhs, float rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs[i] + rhs;
        }
        return result;
    }
    //向量减法运算
    template <int size>
    Vector<size> operator-(const Vector<size> &lhs, const Vector<size> &rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs[i] - rhs[i];
        }
        return result;
    }
    //向量比较运算
    template <int size>
    bool operator==(const Vector<size> &lhs, const Vector<size> &rhs) {
        for (int i = 0; i < size; ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }
        return true;
    }
    //向量比较运算
    template <int size>
    bool operator!=(const Vector<size> &lhs, const Vector<size> &rhs) {
        return !(lhs == rhs);
    }
    // //向量点乘运算
    // template <int size>
    // float operator*(const Vector<size> &lhs, const Vector<size> &rhs) {
    //     float result = 0;
    //     for (int i = 0; i < size; ++i) {
    //         result += lhs[i] * rhs[i];
    //     }
    //     return result;
    // }
    //常数乘向量
    template <int size>
    Vector<size> operator*(const float lhs, const Vector<size> &rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs * rhs[i];
        }
        return result;
    }
    //向量乘常数
    template <int size>
    Vector<size> operator*(const Vector<size> &lhs, const float rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs[i] * rhs;
        }
        return result;
    }
    //向量除常数
    template <int size>
    Vector<size> operator/(const Vector<size> &lhs, const float rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs[i] / rhs;
        }
        return result;
    }
    //向量除常数
    template <int size>
    Vector<size> operator/=(const Vector<size> &lhs, const float rhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = lhs[i] / rhs;
        }
        return result;
    }
    //向量除常数
    template <int size>
    Vector<size> operator-(const Vector<size> &lhs) {
        Vector<size> result;
        for (int i = 0; i < size; ++i) {
            result[i] = -lhs[i];
        }
        return result;
    }
    //
    template <int size1, int size2>
    Vector<size1> embeded(const Vector<size2> &v, const float fill = 1) {
        Vector<size1> result;
        for (int i = 0; i < size1; ++i) {
            result[i] = (i >= size2) ? fill : v[i];
        }
        return result;
    }
    // todo proj
    //向量输出
    template <int size>
    std::ostream &operator<<(std::ostream &out, const Vector<size> v) {
        for (int i = 0; i < size; ++i) {
            std::cout << v[i] << std::endl;
        }
        return out;
    }

    //特化2维向量
    template <>
    struct Vector<2> {
        Vector() = default;
        Vector(float iu, float iv) {
            data[0] = iu;
            data[1] = iv;
        }
        float &operator[](const int i) {
            assert(i >= 0 && i < 2);
            return data[i];
        }
        float operator[](const int i) const {
            assert(i >= 0 && i < 2);
            return data[i];
        }
        float norm2() const {
            return u * u + v * v;
        }
        float norm() const {
            return std::sqrt(norm2());
        }
        Vector normalize() {
            return (*this) / norm();
        }
        Vector normalize() const {
            return (*this) / norm();
        }
        union {
            float data[2];
            struct
            {
                float u;
                float v;
            };
        };
    };
    struct Vector2i {
        Vector2i() = default;
        Vector2i(int iu, int iv) {
            data[0] = iu;
            data[1] = iv;
        }
        int &operator[](const int i) {
            assert(i >= 0 && i < 2);
            return data[i];
        }
        int operator[](const int i) const {
            assert(i >= 0 && i < 2);
            return data[i];
        }
        int norm2() const {
            return u * u + v * v;
        }
        float norm() const {
            return std::sqrt(norm2());
        }
        union {
            int data[2];
            struct
            {
                int u;
                int v;
            };
        };
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
        float &operator[](const int i) {
            assert(i >= 0 && i < 3);
            return data[i];
        }
        float operator[](const int i) const {
            assert(i >= 0 && i < 3);
            return data[i];
        }
        float norm2() const {
            return x * x + y * y + z * z;
        }
        float norm() const {
            return std::sqrt(norm2());
        }
        Vector normalize() {
            return (*this) / norm();
        }
        Vector normalize() const {
            return (*this) / norm();
        }
        float Dot(const Vector<3> &v) {
            return x * v.x + y * v.y + z * v.z;
        }
        float Dot(const Vector<3> &v) const {
            return x * v.x + y * v.y + z * v.z;
        }
        Vector<3> CWiseProduct(const Vector<3> &v) {
            return Vector<3>{x * v.x, y * v.y, z * v.z};
        }
        Vector<3> Cross(const Vector<3> &v) {
            return Vector<3>{y * v.z - z * v.y,
                             z * v.x - x * v.z,
                             x * v.y - y * v.x};
        }
        union {
            float data[3];
            struct
            {
                float x;
                float y;
                float z;
            };
        };
    };
    struct Vector3i {
        Vector3i() = default;
        Vector3i(int ix, int iy, int iz) {
            data[0] = ix;
            data[1] = iy;
            data[2] = iz;
        }
        int &operator[](const int i) {
            assert(i >= 0 && i < 3);
            return data[i];
        }
        int operator[](const int i) const {
            assert(i >= 0 && i < 3);
            return data[i];
        }
        int norm2() const {
            return x * x + y * y + z * z;
        }
        int norm() const {
            return std::sqrt(norm2());
        }

        int Dot(const Vector3i &v) {
            return x * v.x + y * v.y + z * v.z;
        }
        Vector3i CWiseProduct(const Vector3i &v) {
            return Vector3i{x * v.x, y * v.y, z * v.z};
        }
        Vector3i Cross(const Vector3i &v) {
            return Vector3i{y * v.z - z * v.y,
                            z * v.x - x * v.z,
                            x * v.y - y * v.x};
        }
        union {
            int data[3];
            struct
            {
                int x;
                int y;
                int z;
            };
        };
    };
    //特化4维向量
    template <>
    struct Vector<4> {
        Vector() = default;
        Vector(float ix, float iy, float iz, float iw = 0.0f) {
            x = ix;
            y = iy;
            z = iz;
            w = iw;
        }
        float &operator[](const int i) {
            assert(i >= 0 && i < 4);
            return data[i];
        }
        float operator[](const int i) const {
            assert(i >= 0 && i < 4);
            return data[i];
        }
        float norm2() const {
            return x * x + y * y + z * z + w * w;
        }
        float norm() const {
            return std::sqrt(norm2());
        }
        Vector normalize() {
            return (*this) / norm();
        }
        Vector normalize() const {
            return (*this) / norm();
        }
        Vector<3> head3() {
            return Vector<3>{data[0], data[1], data[2]};
        }
        Vector<3> head3() const {
            return Vector<3>{data[0], data[1], data[2]};
        }
        float Dot(const Vector<4> &v) {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }
        float Dot(const Vector<4> &v) const {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }
        Vector<4> CWiseProduct(const Vector<4> &v) {
            return Vector<4>{x * v.x, y * v.y, z * v.z, w * v.w};
        }
        union {
            float data[4];
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };
    };
    typedef Vector<2> Vector2f;
    typedef Vector<3> Vector3f;
    typedef Vector<4> Vector4f;
    Vector<4> ToVector4f(const Vector<3> &vec3, float w);
} // namespace Maths

#endif