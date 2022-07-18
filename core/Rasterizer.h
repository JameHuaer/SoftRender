#ifndef RASTERIZER_H
#define RASTERIZER_H
#include "../math/Vector.h"
#include "../platforms/Win32.h"
#include "Camera.h"
#include "FrameImage.h"
#include "ObjLoader.h"
#include "Shader.h"
#include "Triangle.h"
#include "global.h"
#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <vector>

namespace rst {

//模型填充模式
enum ModelFillMode {
    kWireFrame, //线框模式
    kSolide
};
//多边形裁剪，判断与点在立方体哪里
enum ClipOutCode {
    kInside = 0,
    kTop = 0x01,
    kBottom = 0x02,
    kLeft = 0x04,
    kRight = 0x08,
    kNear = 0x10,
    kFar = 0x20,
};

class Rasterizer {
public:
    Rasterizer(int w, int h);
    ~Rasterizer() {
        if (frame_image_)
            delete frame_image_;
        for (int i = 0; i < triangle_list_.size(); ++i) {
            if (triangle_list_[i])
                delete triangle_list_[i];
        }
        if (camera_)
            delete camera_;
    }

    void SetModel(const Maths::Matrix4f &m);
    void SetView(const Maths::Matrix4f &v);
    void SetProjection(const Maths::Matrix4f &p);

    void SetTexture(Texture2D tex);

    void SetVertexShader(std::function<Maths::Vector3f(VertexShaderPayload)> vert_shader);
    void SetFragmentShader(std::function<Maths::Vector3f(FragmentShaderPayload)> frag_shader);

    uint32_t *&GetFrameBuffer();

    void Draw();

    void Update();
    void Render();

    void Resize(int w, int h);

    bool IsBackFaceCulling(const std::array<Maths::Vector3f, 3> &vecs);

    bool IsClipSimple(const Triangle &t);
    void IsCohenSutherLandClip(const Triangle &tri, std::vector<Triangle> &res);
    void GetCrossVertex(const Triangle &t, std::vector<VertexData> &v_out);
    int ComputeOutCode(double x, double y, double z, double w);
    void LerpAndPushVertex(const Triangle &tri, const Maths::Vector3f &v, std::vector<VertexData> &res);

public:
    int width, height;
    PlatForms::Win32Platform *win32_platform_;
    ModelFillMode fill_mode;

private:
    void DrawLine(Maths::Vector3f begin, Maths::Vector3f end);

    void RasterizeTriangle(const Triangle &t, const std::array<Maths::Vector3f, 3> &world_pos);

    // VERTEX SHADER -> MVP -> Clipping -> /.W -> VIEWPORT -> DRAWLINE/DRAWTRI -> FRAGSHADER
    //顶点着色 -> mvp变换 -> 剔除/裁剪 -> 透视除法 -> 视口变换 -> 画线/画三角形 -> 片元着色
private:
    FrameImage *frame_image_;
    std::vector<Triangle *> triangle_list_;
    Camera *camera_;

    Maths::Matrix4f model;
    Maths::Matrix4f view;
    Maths::Matrix4f projection;

    std::optional<Texture2D> texture;

    std::function<Maths::Vector3f(FragmentShaderPayload)> fragment_shader;
    std::function<Maths::Vector3f(VertexShaderPayload)> vertex_shader;
};
}
#endif