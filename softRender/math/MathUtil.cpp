#include "MathUtil.h"
unsigned int MathUtil::RGBToUint(Maths::Vector4f color) {
    uint32_t r = color.x();
    uint32_t g = color.y();
    uint32_t b = color.z();
    return (uint32_t)(r << 16 | g << 8 | b);
}
unsigned int MathUtil::RGBToUint(uint8_t r,
                                   uint8_t g,
                                   uint8_t b,
                                   uint8_t a ) {
    return (uint32_t)(r << 16 | g << 8 | b);
}