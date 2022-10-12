#include "Texture.h"

Texture2D::Texture2D(const std::string &file_name) {
    //读取图像文件
    std::ifstream fp(file_name, std::ios::binary); //读入的文件
    assert(fp);
    long width, height;
    TagBitMapFileHeadr fileHead;                               //位图文件头
    fp.read((char *) (&fileHead), sizeof(TagBitMapFileHeadr));  //从fp中读取TagBitMapFileHeadr信息到fileHead中,同时fp的指针移动
    TagBitMapInfoHeader infoHead;                              //位图信息头
    fp.read((char *) (&infoHead), sizeof(TagBitMapInfoHeader)); //从fp中读取TagBitMapInfoHeader信息到infoHead中,同时fp的指针移动
    width = infoHead.biWidth;
    height = infoHead.biHeight;

    width_ = width;
    height_ = height;
    texture_buffer_ = new Maths::Vector4f *[width_];
    for (int i = 0; i < height_; ++i) {
        texture_buffer_[i] = new Maths::Vector4f[height_];
    }
    char *bmpBuf = new char[width * height * 3];
    fp.seekg(long(sizeof(TagBitMapFileHeadr) + sizeof(TagBitMapInfoHeader)), std::ios::beg); //定位到起始位置
    fp.read(bmpBuf, width * height * 3);
    fp.close();

    char *tmp = bmpBuf;
    uint8_t r, g, b;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            b = *tmp++;
            g = *tmp++;
            r = *tmp++;
            texture_buffer_[x][y] = Maths::Vector4f{r / 255.0f, g / 255.0f, b / 255.0f};
        }
    delete[] bmpBuf;
}

Texture2D::Texture2D(int w, int h)
        : width_(w), height_(h) {
    texture_buffer_ = new Maths::Vector4f *[width_];
    for (int i = 0; i < height_; ++i) {
        texture_buffer_[i] = new Maths::Vector4f[height_];
    }
}

Texture2D::Texture2D(const Texture2D &t)
        : width_(t.width_), height_(t.height_) {

    texture_buffer_ = new Maths::Vector4f *[width_];
    for (int i = 0; i < width_; ++i) {
        texture_buffer_[i] = new Maths::Vector4f[height_];
    }
    for (int i = 0; i < width_; ++i) {
        for (int j = 0; j < height_; ++j) {
            texture_buffer_[i][j] = t.texture_buffer_[i][j];
        }
    }
}

Maths::Vector4f Texture2D::Sample(const Maths::Vector2f tex) {
    //纹理寻址采用d3d中的wrap方式 当坐标大于1时，通过去掉整数部分，根据得到的小数部分来得到纹理值；
    //坐标小于0，则加上一个最小正数，让坐标大于0。
    if (tex.u >= 0 && tex.u <= 1 && tex.v >= 0 && tex.v <= 1) {
        uint32_t u = tex.u * (width_ - 1);
        uint32_t v = tex.v * (height_ - 1);
        return texture_buffer_[u][v];
    } else {
        float u, v;
        if (tex.u > 1)
            u = tex.u - static_cast<int>(tex.u);
        else if (tex.u < 0)
            u = (static_cast<int>(-tex.u + 1) + tex.u);
        if (tex.v > 1)
            v = tex.v - static_cast<int>(tex.v);
        else if (tex.v < 0)
            v = (static_cast<int>(-tex.v + 1) + tex.v);
        uint32_t u_index = u * (width_ - 1);
        uint32_t v_index = v * (height_ - 1);
        return texture_buffer_[u_index][v_index];
    }
}

Maths::Vector4f Texture2D::GetColor(float u, float v) {

    uint32_t u_img = u * width_;
    uint32_t v_img = v * height_;
    u_img = MathUtil::Clamp(u_img, 0, width_ - 1);
    v_img = MathUtil::Clamp(v_img, 0, height_ - 1);

    return texture_buffer_[u_img][v_img];
}

Maths::Vector3f Texture2D::GetColorBilinear(float u, float v) {
    if (u < 0)
        u = 0;
    if (u > 1)
        u = 1;
    if (v < 0)
        v = 0;
    if (v > 1)
        v = 1;

    float u_img = u * width_;
    float v_img = v * height_;
    //防止访问图片数组越界
    int u_min = std::min((float) width_ - 1, std::floor(u_img));
    int u_max = std::min((float) width_ - 1, std::ceil(u_img));

    int v_min = std::min((float) height_ - 1, std::floor(v_img));
    int v_max = std::min((float) height_ - 1, std::ceil(v_img));

    Maths::Vector4f Q11 = texture_buffer_[u_min][v_max];
    Maths::Vector4f Q12 = texture_buffer_[u_max][v_max];

    Maths::Vector4f Q21 = texture_buffer_[u_min][v_min];
    Maths::Vector4f Q22 = texture_buffer_[u_max][v_min];

    float rs = (u_img - u_min) / (u_max - u_min);
    float rt = (v_img - v_max) / (v_min - v_max);

    Maths::Vector4f cBot = (1 - rs) * Q11 + rs * Q12;
    Maths::Vector4f cTop = (1 - rs) * Q21 + rs * Q22;

    Maths::Vector4f p = (1 - rt) * cBot + rt * cTop;

    return Maths::Vector3f(p[0], p[1], p[2]);
}