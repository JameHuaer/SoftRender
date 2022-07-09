#ifndef MATHUTIL_H
#define MATHUTIL_H
#include "Vector.h"
namespace MathUtil {
// RGB颜色转unsigned int
unsigned int RGBToUint(Maths::Vector4f color);
unsigned int RGBToUint(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

}  // namespace MathUtil

#endif