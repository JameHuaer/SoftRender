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

class rasterizer {
public:
    rasterizer(int w, int h);
    ~rasterizer() {
        if (frame_image_)
            delete frame_image_;
        for (int i = 0; i < triangle_list_.size(); ++i) {
            if (triangle_list_[i])
                delete triangle_list_[i];
        }
        if (camera_)
            delete camera_;
    }

    void set_model(const Maths::Matrix4f &m);
    void set_view(const Maths::Matrix4f &v);
    void set_projection(const Maths::Matrix4f &p);

    void set_texture(Texture2D tex) {
        texture = tex;
    }

    void set_vertex_shader(std::function<Maths::Vector3f(vertex_shader_payload)> vert_shader);
    void set_fragment_shader(std::function<Maths::Vector3f(fragment_shader_payload)> frag_shader);

    // void set_pixel(const Maths::Vector2i &point, const Maths::Vector3f &color);
    void draw();

    // std::vector<Maths::Vector3f> &frame_buffer() {
    //     return frame_buf;
    // }
    uint32_t *&GetFrameBuffer() {
        return frame_image_->GetFrameBuffer();
    }
    void Update();
    void Render();
    void Resize(int w, int h);

public:
    int width, height;
    PlatForms::Win32Platform *win32_platform_;
    ModelFillMode fill_mode;

private:
    void draw_line(Maths::Vector3f begin, Maths::Vector3f end);

    void rasterize_triangle(const Triangle &t, const std::array<Maths::Vector3f, 3> &world_pos);

    // VERTEX SHADER -> MVP -> Clipping -> /.W -> VIEWPORT -> DRAWLINE/DRAWTRI -> FRAGSHADER

private:
    FrameImage *frame_image_;
    std::vector<Triangle *> triangle_list_;
    Camera *camera_;

    Maths::Matrix4f model;
    Maths::Matrix4f view;
    Maths::Matrix4f projection;

    std::optional<Texture2D> texture;

    std::function<Maths::Vector3f(fragment_shader_payload)> fragment_shader;
    std::function<Maths::Vector3f(vertex_shader_payload)> vertex_shader;
};
}
#endif