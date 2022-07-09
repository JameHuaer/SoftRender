#ifndef FRAMEIMAGE_H
#define FRAMEIMAGE_H
#include <iostream>
#include <limits>
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#define FLOAT_MAX 3.402823E38
class FrameImage {
   public:
    FrameImage(int w, int h);
    ~FrameImage();
    void ClearBuffer(Maths::Vector4f color);
    uint32_t* &GetFrameBuffer() { return frame_buffer; }

   private:
    int width;               //图片宽
    int height;              //图片高
    uint32_t* frame_buffer;  //帧图片颜色缓存
    float** z_buffer;        //帧图片深度信息缓存
};

FrameImage::FrameImage(int w, int h) : width(w), height(h) {
    z_buffer = new float*[width];
    for (int i = 0; i < width; ++i) {
        z_buffer[i] = new float[height];
        for (int j = 0; j < height; ++j) {
            z_buffer[i][j] = FLOAT_MAX;
        }
    }
}
FrameImage::~FrameImage() {
    if (frame_buffer)
        delete frame_buffer;
    if (z_buffer)
        for (int i = 0; i < width; ++i) {
            delete[] z_buffer[i];
        }
}
void FrameImage::ClearBuffer(Maths::Vector4f color) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            frame_buffer[width * y + x] = MathUtil::RGBToUint(color);
            z_buffer[x][y] = 0;
        }
    }
}
#endif