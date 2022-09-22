#include "RenderUtil.h"
bool IsBackFaceCulling(const std::array<Maths::Vector3f, 3> &vecs) {
    Maths::Vector3f vec1 = (vecs[1] - vecs[0]);
    Maths::Vector3f vec2 = (vecs[2] - vecs[1]);
    Maths::Vector3f view_direction = vecs[1]; //摄像机默认设置在原点
    Maths::Vector3f n = vec1.Cross(vec2);
    if (view_direction.Dot(n) > 0.0f)
        return true;

    return false;
}