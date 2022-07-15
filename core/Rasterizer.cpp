//
// Created by goksu on 4/6/19.
//

#include "Rasterizer.h"
#include <algorithm>
#include <math.h>
#include <vector>

// Bresenham's line drawing algorithm
void rst::rasterizer::draw_line(Maths::Vector3f begin, Maths::Vector3f end) {
    auto x1 = begin.x;
    auto y1 = begin.y;
    auto x2 = end.x;
    auto y2 = end.y;

    Maths::Vector3f line_color = {255, 255, 255};

    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = fabs(dx);
    dy1 = fabs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;

    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = x1;
            y = y1;
            xe = x2;
        } else {
            x = x2;
            y = y2;
            xe = x1;
        }
        Maths::Vector2i point = Maths::Vector2i(x, y);
        frame_image_->DrawPixel(point.u, point.v, MathUtil::RGBToUint(line_color));
        for (i = 0; x < xe; i++) {
            x = x + 1;
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                } else {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            //            delay(0);
            Maths::Vector2i point = Maths::Vector2i(x, y);
            frame_image_->DrawPixel(point.u, point.v, MathUtil::RGBToUint(line_color));
        }
    } else {
        if (dy >= 0) {
            x = x1;
            y = y1;
            ye = y2;
        } else {
            x = x2;
            y = y2;
            ye = y1;
        }
        Maths::Vector2i point = Maths::Vector2i(x, y);
        frame_image_->DrawPixel(point.u, point.v, MathUtil::RGBToUint(line_color));
        for (i = 0; y < ye; i++) {
            y = y + 1;
            if (py <= 0) {
                py = py + 2 * dx1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                } else {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            //            delay(0);
            Maths::Vector2i point = Maths::Vector2i(x, y);
            frame_image_->DrawPixel(point.u, point.v, MathUtil::RGBToUint(line_color));
        }
    }
}

auto to_vec4(const Maths::Vector3f &v3, float w = 1.0f) {
    return Maths::Vector4f(v3.x, v3.y, v3.z, w);
}

static bool insideTriangle(int x, int y, const Maths::Vector4f *_v) {
    Maths::Vector3f v[3];
    for (int i = 0; i < 3; i++)
        v[i] = {_v[i].x, _v[i].y, 1.0};
    Maths::Vector3f f0, f1, f2;
    f0 = v[1].Cross(v[0]);
    f1 = v[2].Cross(v[1]);
    f2 = v[0].Cross(v[2]);
    Maths::Vector3f p(x, y, 1.);
    if ((p.Dot(f0) * f0.Dot(v[2]) > 0) && (p.Dot(f1) * f1.Dot(v[0]) > 0) && (p.Dot(f2) * f2.Dot(v[1]) > 0))
        return true;
    return false;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Maths::Vector4f *v) {
    float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
    return {c1, c2, c3};
}

void rst::rasterizer::draw() {

    float f1 = (camera_->perspective_arg.z_far - camera_->perspective_arg.z_near) / 2.0;
    float f2 = (camera_->perspective_arg.z_far + camera_->perspective_arg.z_near) / 2.0;

    Maths::Matrix4f mvp = projection * view * model;

    for (const auto &t : triangle_list_) {
        Triangle newtri = *t;

        std::array<Maths::Vector4f, 3> mm{
            (view * model * t->v[0]),
            (view * model * t->v[1]),
            (view * model * t->v[2])};

        std::array<Maths::Vector3f, 3> viewspace_pos;
        viewspace_pos[0] = mm[0].head3();
        viewspace_pos[1] = mm[1].head3();
        viewspace_pos[2] = mm[2].head3();

        Maths::Vector4f v[] = {
            mvp * t->v[0],
            mvp * t->v[1],
            mvp * t->v[2]};
        // Homogeneous division
        for (auto &vec : v) {
            vec.x /= vec.w;
            vec.y /= vec.w;
            vec.z /= vec.w;
        }

        Maths::Matrix4f inv_trans = (view * model).Invert().Transpose();
        Maths::Vector4f n[] = {
            inv_trans * to_vec4(t->normal[0], 0.0f),
            inv_trans * to_vec4(t->normal[1], 0.0f),
            inv_trans * to_vec4(t->normal[2], 0.0f)};

        // Viewport transformation
        for (auto &vert : v) {
            vert.x = 0.5 * width * (vert.x + 1.0);
            vert.y = 0.5 * height * (vert.y + 1.0);
            vert.z = vert.z * f1 + f2;
        }

        for (int i = 0; i < 3; ++i) {
            // screen space coordinates
            newtri.setVertex(i, v[i]);
        }

        for (int i = 0; i < 3; ++i) {
            // view space normal
            newtri.setNormal(i, n[i].head3()); //向量缩小到3维
        }

        newtri.setColor(0, 148, 121.0, 92.0);
        newtri.setColor(1, 148, 121.0, 92.0);
        newtri.setColor(2, 148, 121.0, 92.0);

        fill_mode = kSolide;
        switch (fill_mode) {
        case kWireFrame:
            // BresenhamDrawLine(v1.view_projection.x(), v1.view_projection.y(), v2.view_projection.x(), v2.view_projection.y()); //
            // BresenhamDrawLine(v1.view_projection.x(), v1.view_projection.y(), v3.view_projection.x(), v3.view_projection.y()); //上面
            // BresenhamDrawLine(v2.view_projection.x(), v2.view_projection.y(), v3.view_projection.x(), v3.view_projection.y()); //下面
            break;
        case kSolide:
            // RasterizeTriangle(v1, v2, v3, view_pos);
            // drawPrimitiveScanLine(v1, v2, v3);
            rasterize_triangle(newtri, viewspace_pos);
            break;
        default:
            break;
        }
    }
}

static Maths::Vector3f interpolate(float alpha, float beta, float gamma, const Maths::Vector3f &vert1, const Maths::Vector3f &vert2, const Maths::Vector3f &vert3, float weight) {
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

static Maths::Vector2f interpolate(float alpha, float beta, float gamma, const Maths::Vector2f &vert1, const Maths::Vector2f &vert2, const Maths::Vector2f &vert3, float weight) {
    auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
    auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

    u /= weight;
    v /= weight;

    return Maths::Vector2f(u, v);
}

// Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle &t, const std::array<Maths::Vector3f, 3> &view_pos) {

    auto v = t.toVector4();
    float lmin = INT_MAX, rmax = INT_MIN, bmin = INT_MAX, tmax = INT_MIN, alpha, beta, gamma;
    for (const auto &k : v) {
        lmin = (std::min)(lmin, k.x);
        rmax = (std::max)(rmax, k.x);
        bmin = (std::min)(bmin, k.y);
        tmax = (std::max)(tmax, k.y);
    }
    lmin = std::floor(lmin);
    rmax = std::ceil(rmax);
    bmin = std::floor(bmin);
    tmax = std::ceil(tmax);
    for (int x = lmin; x < rmax; ++x) {
        for (int y = bmin; y < tmax; ++y) {
            int id = frame_image_->GetIndex(x, y);
            if (id < 0 || id>width * height || x >= width || x < 0 || y >= height || y < 0)//上下左右越界的不进行着色，缩放超出屏幕也不进行着色
                continue;
            if (insideTriangle(x, y, t.v)) {
                std::tie(alpha, beta, gamma) = computeBarycentric2D(x, y, t.v);
                float w_reciprocal = 1.0 / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                z_interpolated *= w_reciprocal;
                if (z_interpolated < frame_image_->GetDepth(x, y)) {
                    auto interpolater_color = interpolate(alpha, beta, gamma, t.color[0], t.color[1], t.color[2], 1);                    //颜色插值
                    auto interpolater_normal = interpolate(alpha, beta, gamma, t.normal[0], t.normal[1], t.normal[2], 1);                //法向量插值
                    auto interpolater_texcoords = interpolate(alpha, beta, gamma, t.tex_coords[0], t.tex_coords[1], t.tex_coords[2], 1); //纹理坐标插值
                    auto interpolater_shadingcoords = interpolate(alpha, beta, gamma, view_pos[0], view_pos[1], view_pos[2], 1);         //着色点坐标插值

                    fragment_shader_payload payload(interpolater_color, interpolater_normal.normalize(), interpolater_texcoords, texture ? &*texture : nullptr);
                    payload.view_pos = interpolater_shadingcoords;
                    frame_image_->SetDepth(x, y, z_interpolated);
                    frame_image_->GetFrameBuffer()[id] = MathUtil::RGBToUint(fragment_shader(payload));
                }
            }
        }
    }
}

void rst::rasterizer::set_model(const Maths::Matrix4f &m) {
    model = m;
}

void rst::rasterizer::set_view(const Maths::Matrix4f &v) {
    view = v;
}

void rst::rasterizer::set_projection(const Maths::Matrix4f &p) {
    projection = p;
}

rst::rasterizer::rasterizer(int w, int h)
    : width(w), height(h), frame_image_(new FrameImage(w, h)), camera_(new Camera()) {
    win32_platform_ = new PlatForms::Win32Platform(camera_);

    std::string obj_path = R"(E:\Learning\MyRender\SoftRenderYLQ\models\spot_triangulated_good.obj)";
    // std::string obj_path = R"(E:\Learning\MyRender\SoftRenderYLQ\models\AnyConv.com__Bee.obj)";
    // std::string obj_path = R"(E:\Learning\MyRender\SoftRenderYLQ\models\cube.obj)";
    std::string texture_path = R"(E:\Learning\MyRender\SoftRenderYLQ\models\spot_texture.bmp)";
    //将obj数据存放在triangle中.
    ObjLoader obj_load;
    obj_load.LoadFile(obj_path);
    int vertex_size = obj_load.obj_data_.facet_vrt.size();
    for (int i = 0; i < vertex_size; i += 3) {
        Triangle *t = new Triangle();
        for (int j = 0; j < 3; j++) {
            t->setVertex(j, Maths::ToVector4f(obj_load.obj_data_.verts[obj_load.obj_data_.facet_vrt[i + j]], 1.0f));
            t->setNormal(j, obj_load.obj_data_.norms[obj_load.obj_data_.facet_nrm[i + j]]);
            t->setTexCoord(j, obj_load.obj_data_.tex_coord[obj_load.obj_data_.facet_tex[i + j]]);
        }
        triangle_list_.push_back(t);
    }
    //加载纹理贴图
    set_texture(Texture2D(texture_path));
    //  设置顶点着色模型和片元着色模型
    set_fragment_shader(texture_fragment_shader);
    //设置摄像机参数
    camera_->perspective_arg = PerspectiveArg(45.0, 1.0, 0.1, 50);
}

void rst::rasterizer::set_vertex_shader(std::function<Maths::Vector3f(vertex_shader_payload)> vert_shader) {
    vertex_shader = vert_shader;
}

void rst::rasterizer::set_fragment_shader(std::function<Maths::Vector3f(fragment_shader_payload)> frag_shader) {
    fragment_shader = frag_shader;
}

void rst::rasterizer::Update() {
    frame_image_->ClearBuffer(Maths::Vector4f{0.75f, 0.75f, 0.75f});
    set_model(camera_->get_model_matrix());
    set_view(camera_->get_view_matrix());
    set_projection(camera_->get_projection_matrix());
}

void rst::rasterizer::Render() {
    draw();
}
void rst::rasterizer::Resize(int w, int h) {
    width = w;
    height = h;
    frame_image_->Resize(w, h);
}