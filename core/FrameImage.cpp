#include "FrameImage.h"
const Maths::Vector4f FrameImage::kGray{0.75f, 0.75f, 0.75f};

FrameImage::FrameImage(int w, int h)
    : width(w), height(h) {

    z_buffer = new float *[height]; // width*height;
    for (int i = 0; i < height; ++i) {
        z_buffer[i] = new float[width];
        for (int j = 0; j < width; ++j) {
            z_buffer[i][j] = FLOAT_MAX;
        }
    }
}
FrameImage::~FrameImage() {
    if (frame_buffer)
        delete frame_buffer;
    if (z_buffer)
        for (int i = 0; i < height; ++i) {
            delete[] z_buffer[i];
        }
}
void FrameImage::Resize(int new_width, int new_height) {
    //宽高resize
    //创建一个新的二维数组
    float **new_z_buffer = new float *[new_height];
    for (int i = 0; i < new_height; ++i) {
        new_z_buffer[i] = new float[new_width];
        for (int j = 0; j < new_width; ++j) {
            new_z_buffer[i][j] = FLOAT_MAX;
        }
    }
    //将原本数据拷贝进新数组，中心对齐
    //拷贝交集部分
    int row_start = std::abs(new_height - height) / 2;
    int col_start = std::abs(new_width - width) / 2;
    int row_max = std::min(new_height, height) + row_start;
    int col_max = std::min(new_width, width) + col_start;
    for (int i = row_start; i < row_max; ++i) {
        for (int j = col_start; j < col_max; ++j) {
            if (new_width <= width) {
                if (new_height <= height)
                    new_z_buffer[i - row_start][j - col_start] = z_buffer[i][j]; // 1
                else
                    new_z_buffer[i][j - col_start] = z_buffer[i - row_start][j]; // 2
            } else {
                if (new_height <= height)
                    new_z_buffer[i - row_start][j] = z_buffer[i][j - col_start]; // 3
                else
                    new_z_buffer[i][j] = z_buffer[i - row_start][j - col_start]; // 4
            }
        }
    }
    float **temp_buffer = z_buffer;
    z_buffer = new_z_buffer;
    //释放原来数组内存
    if (temp_buffer)
        for (int i = 0; i < height; ++i) {
            delete[] temp_buffer[i];
        }
    // z_buffer = new_z_buffer; //=新数组
    //保存宽高
    width = new_width;
    height = new_height;
}
void FrameImage::ClearBuffer(const Maths::Vector4f &color) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            frame_buffer[width * y + x] = MathUtil::RGBToUint(color);
            z_buffer[y][x] = FLOAT_MAX;
        }
    }
}
uint32_t *&FrameImage::GetFrameBuffer() {
    return frame_buffer;
}
//画像素点
void FrameImage::DrawPixel(int x, int y, const Maths::Vector4f &color) {
    if (x >= 0 && x < width && y >= 0 && y < height)
    frame_buffer[width * y + x] = MathUtil::RGBToUint(color);
}
void FrameImage::DrawPixel(int x, int y, uint32_t rgb) {
    if (x >= 0 && x < width && y >= 0 && y < height)
    frame_buffer[width * y + x] = rgb;
}
float FrameImage::GetDepth(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height)
    return z_buffer[y][x];
    return 1.0f;
}
void FrameImage::SetDepth(int x, int y, float z) {
    if (x >= 0 && x < width && y >= 0 && y < height)
    z_buffer[y][x] = z;
}
int FrameImage::GetWidth() {
    return width;
}
int FrameImage::GetHeight() {
    return height;
}
int FrameImage::GetIndex(int x, int y) {
    return (height - y - 1) * width + x - 1; 
    // return width * y + x;
}