#ifndef MATHUTIL_H
#define MATHUTIL_H
#include "../core/Texture.h"
// #include "../core/Vertex.h"
#include "../core/Global.h"
#include "Matrix.h"
#include "Vector.h"
#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <vector>

namespace MathUtil {
extern Maths::Vector3f center;

// RGB颜色转unsigned int
unsigned int RGBToUint(Maths::Vector4f color);
unsigned int RGBToUint(Maths::Vector3f color);
float AngleToRadians(float angle);
float Clamp(float x, float min, float max);
Maths::Vector4f Lerp(const Maths::Vector4f &v1, const Maths::Vector4f &v2, float t);
Maths::Vector3f Lerp(const Maths::Vector3f &v1, const Maths::Vector3f &v2, float t);
float Lerp(const float x1, const float x2, const float t);
Maths::Vector2f Lerp(const Maths::Vector2f &v1, const Maths::Vector2f &v2, float t);
bool InsideTriangle(int x, int y, const Maths::Vector4f *_v);
std::tuple<float, float, float> ComputeBarycentric2D(float x, float y, const Maths::Vector4f *v);
Maths::Vector3f Interpolate(float alpha, float beta, float gamma,
                            const Maths::Vector3f &vert1, const Maths::Vector3f &vert2, const Maths::Vector3f &vert3, float weight);
Maths::Vector2f Interpolate(float alpha, float beta, float gamma,
                            const Maths::Vector2f &vert1, const Maths::Vector2f &vert2, const Maths::Vector2f &vert3, float weight);
Maths::Vector4f Interpolate(float alpha, float beta, float gamma,
                            const Maths::Vector4f &vert1, const Maths::Vector4f &vert2, const Maths::Vector4f &vert3, float weight);
//默认使用LessFunction排序，得到的点序为顺时针
void SortPoint(std::vector<VertexData> &vecs);
bool LessFunction(const VertexData &lhs, const VertexData &rhs);
Maths::Vector3f GetCenterPoint(const std::vector<VertexData> &vecs);
Maths::Vector3f ColorConverto(const Maths::Vector3f &cor);
}
#endif