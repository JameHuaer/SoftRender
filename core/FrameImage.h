#ifndef FRAMEIMAGE_H
#define FRAMEIMAGE_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include <iostream>
#include <limits>
#define FLOAT_MAX 3.402823E38

class FrameImage {
public:
    FrameImage(int w, int h);
    ~FrameImage();
    void ClearBuffer(const Maths::Vector4f &color);
    uint32_t *&GetFrameBuffer();
    void DrawPixel(int x, int y, const Maths::Vector4f &color);
    void DrawPixel(int x, int y, uint32_t rgb);
    float GetDepth(int x, int y) const;
    void SetDepth(int x, int y, float z);
    int GetWidth();
    int GetHeight();
    void Resize(int new_width, int new_height);
    int GetIndex(int x, int y);
    static const Maths::Vector4f kGray;

public:
    int width;              //图片宽
    int height;             //图片高
    uint32_t *frame_buffer; //帧图片颜色缓存
    float **z_buffer;       //帧图片深度信息缓存
    int MSAA_rate = 4;      // MSAA

    std::vector<Maths::Vector3f> frame_sample_buffer;
    std::vector<float> z_sample_buffer; //帧图片深度信息缓存
};

#endif