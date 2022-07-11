#ifndef MATRIX_H
#define MATRIX_H
#include "Vector.h"
namespace Maths {

template <int size>
struct dt;

//矩阵
template <int nrows, int ncols>
struct Matrix {
    Vector<ncols> rows[nrows] = {{}};
    Vector<ncols> &operator[](const int idx) {
        assert(idx >= 0 && idx < nrows);
        return rows[idx];
    }
    const Vector<ncols> &operator[](const int idx) const {
        assert(idx >= 0 && idx < nrows);
        return rows[idx];
    }
    //获取列
    Vector<ncols> GetColumn(const int idx) const {
        assert(idx >= 0 && idx < ncols);
        Vector<nrows> result;
        for (int i = 0; i < nrows; ++i) {
            result[i] = rows[i][idx];
        }
        return result;
    }
    //设置列
    void SetColumn(const int idx, const Vector<nrows> &v) {
        assert(idx >= 0 && idx < ncols);
        for (int i = 0; i < nrows; ++i) {
            rows[i][idx] = v[i];
        }
    }
    //获取单位矩阵
    static Matrix<nrows, ncols> Identity() {
        assert(nrows == ncols);
        Matrix<nrows, ncols> result;
        for (int i = 0; i < nrows; ++i) {
            result[i][i] = 1.0f;
        }
        return result;
    }
    //行列式计算
    double Det() const {
        return dt<ncols>::Det(*this);
    }
    //获得子矩阵
    Matrix<nrows - 1, ncols - 1> GetMinor(const int row, const int col) const {
        Matrix<nrows - 1, ncols - 1> result;
        for (int i = 0; i < nrows - 1; ++i) {
            for (int j = 0; j < ncols - 1; ++j) {
                result[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
            }
        }
        return result;
    }
    //计算余子式
    double Cofactor(const int row, const int col) const {
        return GetMinor(row, col).Det() * ((row + col) % 2 ? -1 : 1);
    }
    //计算伴随矩阵
    Matrix<nrows, ncols> Adjugate() const {
        Matrix<nrows, ncols> result;
        for (int i = 0; i < nrows; ++i) {
            for (int j = 0; j < ncols; ++j) {
                result[i][j] = Cofactor(i, j);
            }
        }
        return result;
    }
    //矩阵转置
    Matrix<ncols, nrows> Transpose() const {
        Matrix<ncols, nrows> result;
        for (int i = 0; i < ncols; ++i) {
            result[i] = this->GetColumn(i);
        }
        return result;
    }
    //矩阵的逆
    Matrix<nrows, ncols> Invert() const {
        Matrix<nrows, ncols> result = Adjugate();
        result = result / (result[0] * rows[0]);
        return result.Transpose();
    }
};
// //矩阵乘向量
// template <int nrows, int ncols>
// Vector<nrows> operator*(const Matrix<nrows, ncols> &lhs,
//                         const Vector<ncols> &rhs) {
//     Vector<nrows> result;
//     for (int i = 0; i < nrows; ++i) {
//         result[i] = lhs[i] * rhs;
//     }
//     return result;
// }
//矩阵乘向量,兼容行优先和列优先运算
template <int nrows, int ncols>
Vector<nrows> operator*(const Vector<ncols> &lhs, const Matrix<nrows, ncols> &rhs) {
    Vector<nrows> result;
    for (int i = 0; i < nrows; ++i) {
        result[i] = rhs.GetColumn(i) * lhs;
    }
    return result;
}
//矩阵相乘
template <int r1, int c1, int c2>
Matrix<r1, c2> operator*(const Matrix<r1, c1> &lhs, const Matrix<c1, c2> &rhs) {
    Matrix<r1, c2> result;
    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            result[i][j] = lhs[i] * rhs.GetColumn(j);
        }
    }
    return result;
}
//矩阵乘常数
template <int nrows, int ncols>
Matrix<nrows, ncols> operator*(const Matrix<nrows, ncols> &lhs,
                               const float val) {
    Matrix<nrows, ncols> result;
    for (int i = 0; i < nrows; ++i) {
        result[i] = lhs[i] * val;
    }
    return result;
}
//常数乘矩阵
template <int nrows, int ncols>
Matrix<nrows, ncols> operator*(const float val,
                               const Matrix<nrows, ncols> &rhs) {
    Matrix<nrows, ncols> result;
    for (int i = 0; i < nrows; ++i) {
        result[i] = rhs[i] * val;
    }
    return result;
}
//矩阵除常数
template <int nrows, int ncols>
Matrix<nrows, ncols> operator/(const Matrix<nrows, ncols> &lhs,
                               const float val) {
    assert(val != 0);
    Matrix<nrows, ncols> result;
    for (int i = 0; i < nrows; ++i) {
        result[i] = lhs[i] / val;
    }
    return result;
}
//矩阵相加
template <int nrows, int ncols>
Matrix<nrows, ncols> operator+(const Matrix<nrows, ncols> &lhs,
                               const Matrix<nrows, ncols> &rhs) {
    Matrix<nrows, ncols> result;
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            result[i][j] = lhs[i][j] + rhs[i][j];
        }
    }
    return result;
}
//矩阵相减
template <int nrows, int ncols>
Matrix<nrows, ncols> operator-(const Matrix<nrows, ncols> &lhs,
                               const Matrix<nrows, ncols> &rhs) {
    Matrix<nrows, ncols> result;
    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            result[i][j] = lhs[i][j] - rhs[i][j];
        }
    }
    return result;
}
//输出矩阵
template <int nrows, int ncols>
std::ostream &operator<<(std::ostream &out, const Matrix<nrows, ncols> &mat) {
    for (int i = 0; i < nrows; ++i) {
        out << mat[i] << std::endl;
    }
    return out;
}
//行列式
template <int size>
struct dt {
    static float Det(const Matrix<size, size> &mat) {
        float result = 0.0f;
        for (int i = 0; i < size; ++i) {
            result += mat[0][i] * mat.Cofactor(0, i);
        }
        return result;
    }
};
//行列式递归计算末项
template <>
struct dt<1> {
    static float Det(const Matrix<1, 1> &mat) {
        return mat[0][0];
    }
};

typedef Matrix<2, 2> Matrix2f;
typedef Matrix<3, 3> Matrix3f;
typedef Matrix<4, 4> Matrix4f;

} // namespace Maths
#endif