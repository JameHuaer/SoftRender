#ifndef RASTERIZER_GLOBAL_H
#define RASTERIZER_GLOBAL_H

typedef unsigned char u08;
#define MY_PI 3.1415926
#define TWO_PI (2.0 * MY_PI)
//模型填充模式
enum ModelFillMode {
    kWireFrame, //线框模式
    kSolide
};

//顶点数据结构
struct VertexData {
    VertexData() = default;

    VertexData(const Maths::Vector4f &v, const Maths::Vector3f &n, const Maths::Vector2f &t) {
        vertex = v;
        normal = n;
        tex = t;
    }

    VertexData(const Maths::Vector4f &v) {
        vertex = v;
    }

    Maths::Vector4f vertex;
    Maths::Vector3f normal;
    Maths::Vector2f tex;
};

#endif // RASTERIZER_GLOBAL_H
