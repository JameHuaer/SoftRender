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