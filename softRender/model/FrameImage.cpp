#include "FrameImage.h"
using namespace ModelSpace;
void FrameImage::ClearBuffer(const Maths::Vector4f &color) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            frame_buffer[width * y + x] = MathUtil::RGBToUint(color);
            z_buffer[x][y] = 0;
        }
    }
}
FrameImage::FrameImage(int w, int h)
    : width(w), height(h) {
    z_buffer = new float *[width];
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
uint32_t *&FrameImage::GetFrameBuffer() {
    return frame_buffer;
}
//画像素点
void FrameImage::DrawPixel(int x, int y, const Maths::Vector4f &color) {
    frame_buffer[width * y + x] = MathUtil::RGBToUint(color);
}
float FrameImage::GetDepth(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return z_buffer[x][y];
    return 1.0f;
}
void FrameImage::SetDepth(int x, int y, float z) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        z_buffer[x][y] = z;
}
int FrameImage::GetWidth() {
    return width;
}
int FrameImage::GetHeight() {
    return height;
}