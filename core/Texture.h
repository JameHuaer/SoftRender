#ifndef TEXTURE_H
#define TEXTURE_H
#include "../math/Vector.h"
#include <fstream>
#include <iostream>
#include <string>
#pragma pack(push, 1)
struct TagBitMapFileHeadr {
    uint16_t bfType;      //文件类型，必须为BM(1-2字节）
    uint32_t bfSize;      //文件大小，以字节为单位（3-6字节，低位在前）
    uint16_t bfReserved1; //保留字，必须为0(7-8字节）
    uint16_t bfReserved2; //保留字，必须为0(9-10字节）
    uint32_t bfOffBits;   //位图数据的起始位置，以相对于位图（11-14字节，低位在前）
};
#pragma pack(pop)
#pragma pack(push, 1)
struct TagBitMapInfoHeader {
    uint32_t biSize;          //本结构所占用字节数（15-18字节）
    uint32_t biWidth;         //位图的宽度（19-22字节）
    uint32_t biHeight;        //位图的高度（23-26字节）
    uint16_t biPlanes;        //目标设备的级别，必须为1(27-28字节）
    uint16_t biBitCount;      //每个像素所需的位数，必须是1（双色），4(16色），8(256色）16(高彩色)或24（真彩色）之一（29-30字节）
    uint32_t biCompression;   //位图压缩类型，必须是0（不压缩），1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一（31-34字节）
    uint32_t biSizeImage;     //位图的大小(4字节对齐)，以字节为单位（35-38字节）
    uint32_t biXPelsPerMeter; //位图水平分辨率，每米像素数（39-42字节）
    uint32_t biYPelsPerMeter; //位图垂直分辨率，每米像素数（43-46字节)
    uint32_t biClrUsed;       //位图实际使用的颜色表中的颜色数（47-50字节）
    uint32_t biClrImportant;  //位图显示过程中重要的颜色数（51-54字节）
};
#pragma pack(pop)

class Texture2D {
public:
    Texture2D() = default;
    Texture2D(int w, int h);
    Texture2D(const std::string &file_name);
    Texture2D(const Texture2D &t);
    Texture2D &operator=(const Texture2D &rhs) {
        if (this == &rhs)
            return *this;
        width_ = rhs.width_;
        height_ = rhs.height_;

        texture_buffer_ = new Maths::Vector4f *[width_];
        for (int i = 0; i < width_; ++i) {
            texture_buffer_[i] = new Maths::Vector4f[height_];
        }
        for (int i = 0; i < width_; ++i) {
            for (int j = 0; j < height_; ++j) {
                texture_buffer_[i][j] = rhs.texture_buffer_[i][j];
            }
        }
        return *this;
    }
    ~Texture2D() {
        if (texture_buffer_) {
            for (int i = 0; i < width_; ++i) {
                delete[] texture_buffer_[i];
            }
        }
    }
    Maths::Vector4f GetColor(float u, float v);
    Maths::Vector4f Sample(const Maths::Vector2f tex);
    Maths::Vector3f getColorBilinear(float u, float v);

public:
    int width_;
    int height_;
    Maths::Vector4f **texture_buffer_;
};
#endif