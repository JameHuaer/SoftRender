#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "../math/Vector.h"
#include "../platforms/Win32.h"
#include "Camera.h"
#include "FrameImage.h"
#include "ObjLoader.h"
#include "Shader.h"
#include "ShadowMapping.h"
#include "Triangle.h"
#include "global.h"
#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <vector>
#include "../Config/MainConfig.h"

namespace rst {

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
            for (int i = 0; i < obj_list_.main_obj_.size(); ++i) {
                if (obj_list_.main_obj_[i])
                    delete obj_list_.main_obj_[i];
            }
            for (int i = 0; i < obj_list_.floor_.size(); ++i) {
                if (obj_list_.floor_[i])
                    delete obj_list_.floor_[i];
            }
            if (camera_)
                delete camera_;
            if (shadowMapping_)
                delete shadowMapping_;
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

        //窗口尺寸变化时resize
        void Resize(int w, int h);

        //简单CVV裁剪
        bool IsClipSimple(const Triangle &t);

        // CohenSutherLand多边形裁剪
        void IsCohenSutherLandClip(const Triangle &tri, std::vector<Triangle> &res);

        //多边形裁剪获得与正方体焦点
        void GetCrossVertex(const Triangle &t, std::vector<VertexData> &v_out);

        //计算三角形顶点在正方体哪侧
        int ComputeOutCode(double x, double y, double z, double w);

        //插值新点，并push到点集中
        void LerpAndPushVertex(const Triangle &tri, const Maths::Vector3f &v, std::vector<VertexData> &res);

    public:
        int width, height;
        PlatForms::Win32Platform *win32_platform_;
        ModelFillMode fill_mode; //填充模式
        std::function<Maths::Vector3f(FragmentShaderPayload)> fragment_shader;
        std::function<Maths::Vector3f(VertexShaderPayload)> vertex_shader;
        std::optional<Texture2D> texture;
        std::optional<Texture2D> normalTexture;
        std::optional<Texture2D> bumpTexture;
    private:
        void InitFloor();

        void DrawLine(Maths::Vector3f begin, Maths::Vector3f end);

        void RasterizeTriangle(const Triangle &t, const std::array<Maths::Vector3f, 3> &world_pos);

        void RasterizeTriangleMSAA(const Triangle &t, const std::array<Maths::Vector3f, 3> &view_pos);
        // VERTEX SHADER -> MVP -> Clipping -> /.W -> VIEWPORT -> DRAWLINE/DRAWTRI -> FRAGSHADER
        //顶点着色 -> mvp变换 -> 剔除/裁剪 -> 透视除法 -> 视口变换 -> 画线/画三角形 -> 片元着色
    private:
        FrameImage *frame_image_;
//        std::vector<Triangle *> triangle_list_;
//        std::vector<Triangle *> floor_trigangle_list_;
        ObjList obj_list_;
        Camera *camera_;
        ShadowMapping *shadowMapping_;

        Maths::Matrix4f model;
        Maths::Matrix4f view;
        Maths::Matrix4f projection;
        Maths::Matrix4f invMvp;
        float f1;
        float f2;
        const Maths::Vector4f background_color_{0.6, 0.6, 0.6};
        const Maths::Vector4f floor_color_{0.4, 0.4, 0.4};


        void DrawObj(const std::vector<Triangle *> &triangle_list, bool isFloor);

        void RasterizeFloor(const Triangle &t, const std::array<Maths::Vector3f, 3> &view_pos);
    };
}
#endif