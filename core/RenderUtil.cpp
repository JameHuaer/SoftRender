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

Maths::Matrix4f LookAt(Maths::Vector3f eye, Maths::Vector3f center, Maths::Vector3f up) {
    Maths::Vector3f z = (center - eye).normalize();
    Maths::Vector3f x = z.Cross(up).normalize();
    Maths::Vector3f y = x.Cross(z).normalize();
    Maths::Matrix4f Minv = {{{x.x, y.x, -z.x, 0},
                             {x.y, y.y, -z.y, 0},
                             {x.z, y.z, -z.z, 0},
                             {0, 0, 0, 1}}};
    Maths::Matrix4f Tr = {{{1, 0, 0, 0},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {-eye.x, -eye.y, -eye.z, 1}}};
    return Minv * Tr;
}
