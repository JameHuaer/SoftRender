#include "MathUtil.h"
using namespace MathUtil;

unsigned int MathUtil::RGBToUint(Maths::Vector4f color) {
    uint32_t r = color.x() * 255;
    uint32_t g = color.y() * 255;
    uint32_t b = color.z() * 255;
    return (uint32_t)(r << 16 | g << 8 | b);
}
unsigned int MathUtil::RGBToUint(float r,
                       float g,
                       float b,
                       float a) {
    uint32_t r255 = r * 255;
    uint32_t g255 = g * 255;
    uint32_t b255 = b * 255;
    uint32_t a255 = a * 255;
    return (uint32_t)(r255 << 16 | g255 << 8 | b255);
}
Maths::Matrix4f MathUtil::ScreenTransformMatrix(int w, int h) {
    return Maths::Matrix4f{{{w / 2.0f, 0, 0, 0},
                            {0, h / 2.0f, 0, 0},
                            {0, 0, 1, 0},
                            {w / 2.0f, h / 2.0f, 0, 1}}};
}
ModelSpace::VertexShadeOut MathUtil::Lerp(const ModelSpace::VertexShadeOut &v1, const ModelSpace::VertexShadeOut &v2, float t) {
    auto a = const_cast<ModelSpace::VertexShadeOut &>(v1);
    return ModelSpace::VertexShadeOut(
        Lerp(v1.world_view, v2.world_view, t),
        Lerp(v1.view_projection, v2.view_projection, t),
        Lerp(v1.normal, v2.normal, t),
        Lerp(v1.color, v2.color, t),
        Lerp(v1.texcoord, v2.texcoord, t),
        Lerp(v1.one_div_z, v2.one_div_z, t));
}
float MathUtil::Lerp(float x1, float x2, float t) {
    return x1 + (x2 - x1) * t;
}
Maths::Vector4f MathUtil::Lerp(const Maths::Vector4f &v1, const Maths::Vector4f &v2, float t) {
    return Maths::Vector4f{
        Lerp(v1.x(), v2.x(), t), Lerp(v1.y(), v2.y(), t), Lerp(v1.z(), v2.z(), t), v1.w()};
}
Maths::Vector2f MathUtil::Lerp(const Maths::Vector2f &v1, const Maths::Vector2f &v2, float t) {
    return Maths::Vector2f{
        Lerp(v1.x(), v2.x(), t), Lerp(v1.y(), v2.y(), t)};
}
Maths::Vector3f MathUtil::Lerp(const Maths::Vector3f &v1, const Maths::Vector3f &v2, float t) {
    return Maths::Vector3f{
        Lerp(v1.x(), v2.x(), t), Lerp(v1.y(), v2.y(), t), Lerp(v1.z(), v2.z(), t)};
}