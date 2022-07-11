#ifndef FRAMEIMAGE_H
#define FRAMEIMAGE_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include <iostream>
#include <limits>
namespace ModelSpace {
#define FLOAT_MAX 3.402823E38

class FrameImage {
public:
    FrameImage(int w, int h);
    ~FrameImage();
    void ClearBuffer(const Maths::Vector4f &color);
    uint32_t *&GetFrameBuffer();
    void DrawPixel(int x, int y, const Maths::Vector4f &color);
    float GetDepth(int x, int y) const;
    void SetDepth(int x, int y, float z);
    int GetWidth();
    int GetHeight();

private:
    int width;              //图片宽
    int height;             //图片高
    uint32_t *frame_buffer; //帧图片颜色缓存
    float **z_buffer;       //帧图片深度信息缓存
};

}

#endif