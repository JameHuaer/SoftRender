#include "MathUtil.h"
using namespace std;

unsigned int MathUtil::RGBToUint(Maths::Vector4f color) {
    uint32_t r = color.x * 255.0f;
    uint32_t g = color.y * 255.0f;
    uint32_t b = color.z * 255.0f;
    return (uint32_t)(r << 16 | g << 8 | b);
}
unsigned int MathUtil::RGBToUint(Maths::Vector3f color) {
    uint32_t r = color.x * 255.0f;
    uint32_t g = color.y * 255.0f;
    uint32_t b = color.z * 255.0f;
    return (uint32_t)(r << 16 | g << 8 | b);
}
float MathUtil::AngleToRadians(float angle) {
    return (angle / 180.0f) * MY_PI;
}
float MathUtil::Clamp(float x, float min, float max) {
    return x <= min ? min : (x >= max ? max : x);
}
Maths::Vector4f MathUtil::Lerp(const Maths::Vector4f &v1, const Maths::Vector4f &v2, float t) {
    return Maths::Vector4f{
        Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t), v1.w};
}
Maths::Vector3f MathUtil::Lerp(const Maths::Vector3f &v1, const Maths::Vector3f &v2, float t) {
    return Maths::Vector3f{
        Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)};
}
float MathUtil::Lerp(float x1, float x2, float t) {
    return x1 + (x2 - x1) * t;
}
Maths::Vector2f MathUtil::Lerp(const Maths::Vector2f &v1, const Maths::Vector2f &v2, float t) {
    return Maths::Vector2f{
        Lerp(v1.u, v2.u, t), Lerp(v1.v, v2.v, t)};
}
bool MathUtil::InsideTriangle(int x, int y, const Maths::Vector4f *_v) {
    Maths::Vector3f v[3];
    for (int i = 0; i < 3; i++)
        v[i] = {_v[i].x, _v[i].y, 1.0};
    Maths::Vector3f f0, f1, f2;
    f0 = v[1].Cross(v[0]);
    f1 = v[2].Cross(v[1]);
    f2 = v[0].Cross(v[2]);
    Maths::Vector3f p(x, y, 1.);
    if ((p.Dot(f0) * f0.Dot(v[2]) > 0) && (p.Dot(f1) * f1.Dot(v[0]) > 0) && (p.Dot(f2) * f2.Dot(v[1]) > 0))
        return true;
    return false;
}
std::tuple<float, float, float> MathUtil::ComputeBarycentric2D(float x, float y, const Maths::Vector4f *v) {
    float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
    return {c1, c2, c3};
}
Maths::Vector4f MathUtil::Interpolate(float alpha, float beta, float gamma,
                                      const Maths::Vector4f &vert1, const Maths::Vector4f &vert2, const Maths::Vector4f &vert3, float weight) {
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}
Maths::Vector3f MathUtil::Interpolate(float alpha, float beta, float gamma,
                                      const Maths::Vector3f &vert1, const Maths::Vector3f &vert2, const Maths::Vector3f &vert3, float weight) {
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

Maths::Vector2f MathUtil::Interpolate(float alpha, float beta, float gamma,
                                      const Maths::Vector2f &vert1, const Maths::Vector2f &vert2, const Maths::Vector2f &vert3, float weight) {
    auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
    auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

    u /= weight;
    v /= weight;

    return Maths::Vector2f(u, v);
}

Maths::Vector3f MathUtil::center;

Maths::Vector3f MathUtil::GetCenterPoint(const std::vector<VertexData> &vecs) {
    float xSum = 0;
    float ySum = 0;
    for (int i = 0; i < vecs.size(); ++i) {
        xSum += vecs[i].vertex.x;
        ySum += vecs[i].vertex.y;
    }
    return Maths::Vector3f{xSum / ((float)vecs.size()), ySum / ((float)vecs.size()), 0};
}

bool MathUtil::LessFunction(const VertexData &lhs, const VertexData &rhs) {
    if (lhs.vertex.x - center.x >= 0 && rhs.vertex.x - center.x < 0)
        return true;
    if (lhs.vertex.x - center.x < 0 && rhs.vertex.x - center.x >= 0)
        return false;
    if (lhs.vertex.x - center.x == 0 && rhs.vertex.x - center.x == 0) {
        if (lhs.vertex.y - center.y >= 0 || rhs.vertex.y - center.y >= 0)
            return lhs.vertex.y > rhs.vertex.y;
        return rhs.vertex.y > lhs.vertex.y;
    }
    float det = (lhs.vertex.x - center.x) * (rhs.vertex.y - center.y) - (rhs.vertex.x - center.x) * (lhs.vertex.y - center.y);
    if (det < 0)
        return true;
    else if (det > 0)
        return false;
    float d1 = (lhs.vertex.x - center.x) * (lhs.vertex.x - center.x) + (lhs.vertex.y - center.y) * (lhs.vertex.y - center.y);
    float d2 = (rhs.vertex.x - center.x) * (rhs.vertex.x - center.x) + (rhs.vertex.y - center.y) * (rhs.vertex.y - center.y);
    return d1 > d2;
}
void MathUtil::SortPoint(std::vector<VertexData> &vecs) {
    center = GetCenterPoint(vecs);
    std::sort(vecs.begin(), vecs.end(), LessFunction);
}
