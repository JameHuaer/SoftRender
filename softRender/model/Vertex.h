#ifndef VERTEX_H
#define VERTEX_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
//顶点信息
struct VertexPrimitive {
    VertexPrimitive() = default;
    VertexPrimitive(const Maths::Vector4f& pos,
                    const Maths::Vector4f& cor,
                    const Maths::Vector2f& tex,
                    const Maths::Vector4f& n) {
        position = pos;
        color = cor;
        texcoord = tex;
        normal = n;
    }
    VertexPrimitive(const VertexPrimitive& vp) {
        position = vp.position;
        color = vp.color;
        texcoord = vp.texcoord;
        normal = vp.normal;
    }
    VertexPrimitive& operator=(const VertexPrimitive& vp) {
        if (this == &vp)
            return *this;
        position = vp.position;
        color = vp.color;
        texcoord = vp.texcoord;
        normal = vp.normal;
    }
    Maths::Vector4f position;  //顶点坐标
    Maths::Vector4f color;     //顶点颜色
    Maths::Vector4f normal;    //顶点法线
    Maths::Vector2f texcoord;  //顶点纹理坐标
};
struct VertexShade {
    Maths::Vector4f world_view;       //世界变换后坐标
    Maths::Vector4f view_projection;  //投影变换后坐标
    Maths::Vector4f color;            //顶点颜色
    Maths::Vector4f normal;           //顶点法线
    Maths::Vector2f texcoord;         //纹理坐标
    float one_div_z;                  // 1/z用于深度测试
    VertexShade() = default;
    VertexShade(const Maths::Vector4f& wv,
                const Maths::Vector4f& vp,
                const Maths::Vector4f& cor,
                const Maths::Vector4f& n,
                const Maths::Vector2f& tex,
                float odz) {
        world_view = wv;
        view_projection = vp;
        color = cor;
        normal = n;
        texcoord = tex;
        one_div_z = odz;
    }
    VertexShade(const VertexShade& vs) {
        world_view = vs.world_view;
        view_projection = vs.view_projection;
        color = vs.color;
        normal = vs.normal;
        texcoord = vs.texcoord;
        one_div_z = vs.one_div_z;
    }
    VertexShade& operator=(const VertexShade& vs) {
        if (this == &vs)
            return *this;
        world_view = vs.world_view;
        view_projection = vs.view_projection;
        color = vs.color;
        normal = vs.normal;
        texcoord = vs.texcoord;
        one_div_z = vs.one_div_z;
        return *this;
    }
};

#endif