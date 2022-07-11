#ifndef MATHUTIL_H
#define MATHUTIL_H
#include "../model/Vertex.h"
#include "Matrix.h"
#include "Vector.h"

namespace MathUtil {
    // RGB颜色转unsigned int
    unsigned int RGBToUint(Maths::Vector4f color);
    unsigned int RGBToUint(float r, float g, float b, float a = 1.0f);
    Maths::Matrix4f ScreenTransformMatrix(int w, int h);
    ModelSpace::VertexShadeOut Lerp(const ModelSpace::VertexShadeOut &v1, const ModelSpace::VertexShadeOut &v2, float t);
    float Lerp(const float x1, const float x2, const float t);
    Maths::Vector4f Lerp(const Maths::Vector4f &v1, const Maths::Vector4f &v2, float t);
    Maths::Vector2f Lerp(const Maths::Vector2f &v1, const Maths::Vector2f &v2, float t);
    Maths::Vector3f Lerp(const Maths::Vector3f &v1, const Maths::Vector3f &v2, float t);

} // namespace MathUtil

#endif