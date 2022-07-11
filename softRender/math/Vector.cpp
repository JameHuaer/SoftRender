#include "Vector.h"
Maths::Vector3f Maths::Cross(const Maths::Vector3f &v1,
                             const Maths::Vector3f &v2) {
    return Maths::Vector<3>{v1.y() * v2.z() - v1.z() * v2.y(),
                            v1.z() * v2.x() - v1.x() * v2.z(),
                            v1.x() * v2.y() - v1.y() * v2.x()};
}
Maths::Vector4f Maths::Cross(const Maths::Vector4f &v1,
                             const Maths::Vector4f &v2) {
    return Maths::Vector<4>{v1.y() * v2.z() - v1.z() * v2.y(),
                            v1.z() * v2.x() - v1.x() * v2.z(),
                            v1.x() * v2.y() - v1.y() * v2.x(), 0.0f};
}