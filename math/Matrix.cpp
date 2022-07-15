#include "Matrix.h"
using namespace Maths;
void Maths::TopLeftConner3(Matrix4f &lhs, const Matrix3f &rhs) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            lhs[i][j] = rhs[i][i];
        }
    }
}
Matrix3f Maths::VectorMulTranspose(const Vector3f &vec) {
    return Matrix3f{{{vec[0] * vec[0], vec[0] * vec[1], vec[0] * vec[2]},
                     {vec[1] * vec[0], vec[1] * vec[1], vec[1] * vec[2]},
                     {vec[2] * vec[0], vec[2] * vec[1], vec[2] * vec[2]}}};
}