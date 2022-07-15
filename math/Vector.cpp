#include "Vector.h"
using namespace Maths;
Vector<4> Maths::ToVector4f(const Vector<3> &vec3, float w) {
    return Vector<4>{vec3.x, vec3.y, vec3.z, w};
}