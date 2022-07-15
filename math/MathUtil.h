#ifndef MATHUTIL_H
#define MATHUTIL_H
#include "../core/Texture.h"
// #include "../core/Vertex.h"
#include "../core/Global.h"
#include "Matrix.h"
#include "Vector.h"
namespace MathUtil {
// RGB颜色转unsigned int
unsigned int RGBToUint(Maths::Vector4f color);
unsigned int RGBToUint(Maths::Vector3f color);
float AngleToRadians(float angle);
float Clamp(float x, float min, float max);
}
#endif