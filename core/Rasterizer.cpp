
#include "Rasterizer.h"
#include "RenderUtil.h"
#include <algorithm>
#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>

using namespace rst;
// std::ofstream outFile("./Debug.txt");

Rasterizer::Rasterizer(int w, int h)
        : width(w), height(h), frame_image_(new FrameImage(w, h)), camera_(new Camera()) {
    win32_platform_ = new PlatForms::Win32Platform(camera_);
    //加载模型
    std::string obj_path = R"(../models/spot_triangulated_good.obj)";
    //  std::string obj_path = R"(../models/Baby Zebra.obj)";
    //  std::string obj_path = R"(../models\AnyConv.com__Bee.obj)";
    //    std::string obj_path = R"(../models\cube.obj)";
    std::string texture_path = R"(../models/spot_texture.bmp)";
    // std::string texture_path = R"(../models/hmap.bmp)";
    //添加地面
    InitFloor();
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
        obj_list_.main_obj_.push_back(t);
    }
    //加载纹理贴图
    SetTexture(Texture2D(texture_path));
    //  设置顶点着色模型和片元着色模型
    SetFragmentShader(TextureFragmentShader);
    //初始化shadowMapping
    Maths::Vector3f lightPosition = {-20, 20, 0};
    Maths::Vector3f lightIntensity = {500, 500, 500};
    shadowMapping_ = new ShadowMapping({{lightPosition, lightIntensity}}, w, h);
    win32_platform_->SetShadowMapping(shadowMapping_);
    //设置摄像机参数
    camera_->perspective_arg_ = PerspectiveArg(45.0, 1.0, 0.1, 50);
    //填充模式
    fill_mode = kSolide;
    IsMSAA = false;
    f1 = (camera_->perspective_arg_.z_far - camera_->perspective_arg_.z_near) / 2.0f; // 24.95
    f2 = (camera_->perspective_arg_.z_far + camera_->perspective_arg_.z_near) / 2.0f; // 25.05
}

void Rasterizer::Update() {
    //    float startTime, endTime;
    //    startTime = clock();
    frame_image_->ClearBuffer(background_color_); //设置背景色
    SetModel(camera_->GetModelMatrix());
    SetView(camera_->GetViewMatrix());
    SetProjection(camera_->GetProjectionMatrix());
    // shadow mapping 更新
    shadowMapping_->ClearZBuffer();
    shadowMapping_->UpdateMVPMatrix();
    shadowMapping_->UpdateShadowMappingDepth(obj_list_);
    //    endTime = clock();
    //    std::cout << "update time: " << (endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
}

void Rasterizer::Render() {
    float startTime, endTime;
    startTime = clock();
    Draw();
    endTime = clock();
    float renderTime = (endTime - startTime) / CLOCKS_PER_SEC;
    std::cout << "render time: " << renderTime << "s" << "  fps: " << 1 / renderTime << std::endl;
}

void Rasterizer::Draw() {
    DrawObj(obj_list_.floor_, true);
    DrawObj(obj_list_.main_obj_, false);
}

void Rasterizer::DrawObj(const std::vector<Triangle *> &triangle_list, bool isFloor) {
    Maths::Matrix4f mvp = projection * view * model;
    Maths::Matrix4f mv = view * model;
    //法线
    Maths::Matrix4f inv_trans = mv.Invert().Transpose();
    invMvp = mvp.Invert();
    std::vector<Triangle> clip_triangle;

    for (const auto &t: triangle_list) {
        Triangle newtri = *t;

        //计算视口矩阵
        std::array<Maths::Vector4f, 3> mm{
                (mv * t->v[0]),
                (mv * t->v[1]),
                (mv * t->v[2])};
        //保存视口坐标
        std::array<Maths::Vector3f, 3> viewspace_pos{};
        viewspace_pos[0] = mm[0].head3();
        viewspace_pos[1] = mm[1].head3();
        viewspace_pos[2] = mm[2].head3();
        //背面剔除
        if (IsBackFaceCulling(viewspace_pos))
            continue;
        //将三角形世界坐标转换到摄像机坐标
        newtri.v[0] = mvp * t->v[0];
        newtri.v[1] = mvp * t->v[1];
        newtri.v[2] = mvp * t->v[2];

        // Homogeneous division 透视除法
        for (auto &vertex: newtri.v) {
            vertex.x /= vertex.w;
            vertex.y /= vertex.w;
            vertex.z /= vertex.w;
        }

        //简单CVV裁剪
        // 简单裁剪，GetCrossVertex
        // X在[-1,1], y在[-1,1], z在[-2,0]
        // if (IsClipSimple(newtri))
        //     continue;
        // else
        //     clip_triangle.push_back(newtri);

        // 多边形裁剪：
        //计算两点与正方体交点
        //多边形裁剪，对每个新的点和法向量进行插值，得到三角形，
        //然后经过视口变换以及法向量变换，重新设置顶点坐标和法向量
        IsCohenSutherLandClip(newtri, clip_triangle);

        for (auto &triangle: clip_triangle) {

            Maths::Vector4f n[] = {
                    inv_trans * Maths::ToVector4f(triangle.normal[0], 0.0f),
                    inv_trans * Maths::ToVector4f(triangle.normal[1], 0.0f),
                    inv_trans * Maths::ToVector4f(triangle.normal[2], 0.0f)};
            // Viewport transformation 视口变换
            for (int j = 0; j < 3; ++j) {
                triangle.v[j].x = 0.5f * width * (triangle.v[j].x + 1.0);
                triangle.v[j].y = 0.5f * height * (triangle.v[j].y + 1.0);
                triangle.v[j].z = triangle.v[j].z * f1 + f2;
            }
            // view space normal
            triangle.setNormals({n[0].head3(), n[1].head3(), n[2].head3()}); //向量缩小到3维

            triangle.setColor(0, 148, 121.0, 92.0);
            triangle.setColor(1, 148, 121.0, 92.0);
            triangle.setColor(2, 148, 121.0, 92.0);

            switch (fill_mode) {
                case kWireFrame:
                    DrawLine(triangle.v[0].head3(), triangle.v[1].head3());
                    DrawLine(triangle.v[0].head3(), triangle.v[2].head3());
                    DrawLine(triangle.v[1].head3(), triangle.v[2].head3());
                    break;
                case kSolide:
                    if (IsMSAA) {
                        if (isFloor)
                            RasterizeFloor(triangle, viewspace_pos);
                        else
                            RasterizeTriangleMSAA(triangle, viewspace_pos);
                    } else {
                        if (isFloor)
                            RasterizeFloor(triangle, viewspace_pos);
                        else
                            RasterizeTriangle(triangle, viewspace_pos);
                    }
                    break;
                default:
                    break;
            }
        }
        clip_triangle.clear();
    }
}

// 屏幕空间光栅化，光栅化三角形。
void Rasterizer::RasterizeTriangle(const Triangle &t, const std::array<Maths::Vector3f, 3> &view_pos) {

    auto v = t.toVector4(); //所有顶点的w值都设为1
    float lmin = INT_MAX, rmax = INT_MIN, bmin = INT_MAX, tmax = INT_MIN, alpha, beta, gamma;
    for (const auto &k: v) {
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
            //上下左右越界的不进行着色，缩放超出屏幕也不进行着色，在进行多边形裁剪后，该语句没有必要
            //            if (id < 0 || id > width * height || x >= width || x < 0 || y >= height || y < 0)
            //                continue;
            if (MathUtil::InsideTriangle(x, y, t.v)) {
                std::tie(alpha, beta, gamma) = MathUtil::ComputeBarycentric2D(x + 0.5f, y + 0.5f, t.v);
                float w_reciprocal = 1.0f / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                z_interpolated *= w_reciprocal;
                if (z_interpolated < frame_image_->GetDepth(x, y)) {
                    auto interpolater_color = MathUtil::Interpolate(alpha, beta, gamma, t.color[0], t.color[1], t.color[2], 1);                    //颜色插值
                    auto interpolater_normal = MathUtil::Interpolate(alpha, beta, gamma, t.normal[0], t.normal[1], t.normal[2], 1);                //法向量插值
                    auto interpolater_texcoords = MathUtil::Interpolate(alpha, beta, gamma, t.tex_coords[0], t.tex_coords[1], t.tex_coords[2], 1); //纹理坐标插值
                    auto interpolater_shadingcoords = MathUtil::Interpolate(alpha, beta, gamma, view_pos[0], view_pos[1], view_pos[2], 1);         //着色点坐标插值

                    FragmentShaderPayload payload(interpolater_color, interpolater_normal.normalize(), interpolater_texcoords, texture ? &*texture : nullptr);
                    payload.view_pos = interpolater_shadingcoords;
                    frame_image_->SetDepth(x, y, z_interpolated);
                    //摄像机屏幕坐标转世界坐标
                    Maths::Vector3f pixelPos = {(float) x, (float) y, z_interpolated};
                    Maths::Vector3f tempPos = {pixelPos.x * 2 / (float) width - 1, pixelPos.y * 2 / (float) height - 1, (pixelPos.z - f2) / f1};
                    float interpW = alpha * v[0].w + beta * v[1].w + gamma * v[2].w;
                    tempPos = tempPos * interpW;
                    Maths::Vector4f homoCoor = Maths::ToVector4f(tempPos, interpW);
                    Maths::Vector4f worldPos = invMvp * homoCoor;
                    bool isInLight = shadowMapping_->IsInLight(worldPos, interpolater_normal);

                    frame_image_->GetFrameBuffer()[id] = isInLight ? MathUtil::RGBToUint(fragment_shader(payload))
                                                                   : MathUtil::RGBToUint(Maths::Vector3f(0.0f, 0.0f, 0.0f));
                }
            }
        }
    }
}

void Rasterizer::RasterizeFloor(const Triangle &t, const std::array<Maths::Vector3f, 3> &view_pos) {

    auto v = t.toVector4(); //所有顶点的w值都设为1
    float lmin = INT_MAX, rmax = INT_MIN, bmin = INT_MAX, tmax = INT_MIN, alpha, beta, gamma;
    for (const auto &k: v) {
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
            //上下左右越界的不进行着色，缩放超出屏幕也不进行着色，在进行多边形裁剪后，该语句没有必要
            //            if (id < 0 || id > width * height || x >= width || x < 0 || y >= height || y < 0)
            //                continue;
            if (MathUtil::InsideTriangle(x, y, t.v)) {
                std::tie(alpha, beta, gamma) = MathUtil::ComputeBarycentric2D(x + 0.5f, y + 0.5f, t.v);
                float w_reciprocal = 1.0f;
                float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                z_interpolated *= w_reciprocal;
                if (z_interpolated < frame_image_->GetDepth(x, y)) {
                    Maths::Vector3f interpolater_normal{0, 1, 0}; //法向量

                    frame_image_->SetDepth(x, y, z_interpolated);
                    //摄像机屏幕坐标转世界坐标
                    Maths::Vector3f pixelPos = {(float) x, (float) y, z_interpolated};
                    Maths::Vector3f tempPos = {pixelPos.x * 2 / (float) width - 1, pixelPos.y * 2 / (float) height - 1, (pixelPos.z - f2) / f1};
                    tempPos = tempPos * w_reciprocal;
                    Maths::Vector4f homoCoor = Maths::ToVector4f(tempPos, w_reciprocal);
                    Maths::Vector4f worldPos = invMvp * homoCoor;
                    bool isInLight = shadowMapping_->IsInLight(worldPos, interpolater_normal);

                    frame_image_->GetFrameBuffer()[id] = isInLight ? MathUtil::RGBToUint(floor_color_)
                                                                   : MathUtil::RGBToUint(Maths::Vector3f(0.0f, 0.0f, 0.0f));
                }
            }
        }
    }
}

// 屏幕空间光栅化
void Rasterizer::RasterizeTriangleMSAA(const Triangle &t, const std::array<Maths::Vector3f, 3> &view_pos) {

    auto v = t.toVector4(); //所有顶点的w值都设为1
    float lmin = INT_MAX, rmax = INT_MIN, bmin = INT_MAX, tmax = INT_MIN, alpha, beta, gamma;
    for (const auto &k: v) {
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
            //使用MSAA
            float min_depth = FLOAT_MAX;
            int sample_k = 0;
            int id = frame_image_->GetIndex(x, y);

            float delta_msaa_rate = 1.0f / sqrt(frame_image_->MSAA_rate);
            //上下左右越界的不进行着色，缩放超出屏幕也不进行着色，在进行多边形裁剪后，该语句没有必要
            //            if (id < 0 || id > width * height || x >= width || x < 0 || y >= height || y < 0)
            //                continue;
            id *= frame_image_->MSAA_rate;

            for (float inner_x = delta_msaa_rate / 2.0f; inner_x < 1; inner_x += delta_msaa_rate) {
                for (float inner_y = delta_msaa_rate / 2.0f; inner_y < 1; inner_y += delta_msaa_rate, ++sample_k) {
                    // for (float inner_x = 0.25; inner_x < 1; inner_x += 0.5) {
                    //     for (float inner_y = 0.25; inner_y < 1; inner_y += 0.5, ++sample_k) {
                    float point_x = x + inner_x;
                    float point_y = y + inner_y;
                    if (MathUtil::InsideTriangle(point_x, point_y, t.v)) {
                        std::tie(alpha, beta, gamma) = MathUtil::ComputeBarycentric2D(point_x, point_y, t.v);
                        float w_reciprocal = 1.0f / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                        float z_interpolated =
                                alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                        z_interpolated *= w_reciprocal;
                        if (z_interpolated < frame_image_->z_sample_buffer[id + sample_k]) {
                            auto interpolater_color = MathUtil::Interpolate(alpha, beta, gamma, t.color[0], t.color[1],
                                                                            t.color[2], 1); //颜色插值
                            auto interpolater_normal = MathUtil::Interpolate(alpha, beta, gamma, t.normal[0],
                                                                             t.normal[1], t.normal[2],
                                                                             1); //法向量插值
                            auto interpolater_texcoords = MathUtil::Interpolate(alpha, beta, gamma, t.tex_coords[0],
                                                                                t.tex_coords[1], t.tex_coords[2],
                                                                                1); //纹理坐标插值
                            auto interpolater_shadingcoords = MathUtil::Interpolate(alpha, beta, gamma, view_pos[0],
                                                                                    view_pos[1], view_pos[2],
                                                                                    1); //着色点坐标插值

                            FragmentShaderPayload payload(interpolater_color, interpolater_normal.normalize(),
                                                          interpolater_texcoords, texture ? &*texture : nullptr);
                            payload.view_pos = interpolater_shadingcoords;
                            frame_image_->z_sample_buffer[id + sample_k] = z_interpolated;
                            frame_image_->frame_sample_buffer[id + sample_k] = fragment_shader(payload) / 4;
                        }
                        min_depth = std::fmin(min_depth, z_interpolated); //记录最小深度
                    }
                }
            }
            Maths::Vector3f color = {0, 0, 0};
            for (int i = 0; i < frame_image_->MSAA_rate; ++i) {
                color = color + frame_image_->frame_sample_buffer[id + i];
            }
            frame_image_->SetDepth(x, y, std::fmin(frame_image_->GetDepth(x, y), min_depth));
            frame_image_->GetFrameBuffer()[frame_image_->GetIndex(x, y)] = MathUtil::RGBToUint(color);
        }
    }
}

// Bresenham's line drawing algorithm
void Rasterizer::DrawLine(Maths::Vector3f begin, Maths::Vector3f end) {
    float x0 = begin.x;
    float y0 = begin.y;
    float x1 = end.x;
    float y1 = end.y;
    Maths::Vector3f line_color = {1, 0, 0};

    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) // [0,width-1],[0,height-1]
            frame_image_->DrawPixel(y == width ? width - 1 : y, (height - x) == height ? height - 1 : (height - x), MathUtil::RGBToUint(line_color));
        else
            frame_image_->DrawPixel(x == width ? width - 1 : x, (height - y) == height ? height - 1 : (height - y), MathUtil::RGBToUint(line_color));
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Rasterizer::Resize(int w, int h) {
    width = w;
    height = h;
    frame_image_->Resize(w, h);
}

//简单cvv裁剪
bool Rasterizer::IsClipSimple(const Triangle &t) {
    //[-1,1]^3，/ X在[-1,1], y在[-1,1], z在[-2,0]
    for (int i = 0; i < 3; ++i) {
        if (abs(t.v[i].x) > 1 || abs(t.v[i].y) > 1 || abs(t.v[i].z) > 1.1f)
            return true;
    }
    return false;
}

int Rasterizer::ComputeOutCode(double x, double y, double z, double w) {
    //判断顶点区域码
    int code = ClipOutCode::kInside; // x,y,z==w 也算在里面
    if (x < -1)
        code |= ClipOutCode::kLeft;
    else if (x > 1)
        code |= ClipOutCode::kRight;
    if (y < -1)
        code |= ClipOutCode::kBottom;
    else if (y > 1)
        code |= ClipOutCode::kTop;
    if (z < -1.1) // modify -1?-1.1
        code |= ClipOutCode::kNear;
    else if (z > 1.1)
        code |= ClipOutCode::kFar;
    return code;
}

void Rasterizer::GetCrossVertex(const Triangle &tri, std::vector<VertexData> &res) {

    std::vector<int> indices = {0, 1, 2, 0};
    std::pair<int, bool> isVertexPush[3] = {{0, false},
                                            {1, false},
                                            {2, false}}; //保存该顶点是否储存到res状态
    //边角点坐标，左手系
    const Maths::Vector3f top_right_vextex = {1, 1, 0};
    const Maths::Vector3f top_left_vextex = {-1, 1, 0};
    const Maths::Vector3f bottom_right_vextex = {1, -1, 0};
    const Maths::Vector3f bottom_left_vextex = {-1, -1, 0};
    // obj文件内三角形顶点顺序是逆时针。
    bool is_top_left_insert = false;
    bool is_top_right_insert = false;
    bool is_bottom_left_insert = false;
    bool is_bottom_right_insert = false;

    //判断正方体x,y边角点是否在三角形内
    bool is_top_left_inside = MathUtil::InsideTriangle(-1, 1, tri.v);
    bool is_top_right_inside = MathUtil::InsideTriangle(1, 1, tri.v);
    bool is_bottom_left_inside = MathUtil::InsideTriangle(-1, -1, tri.v);
    bool is_bottom_right_inside = MathUtil::InsideTriangle(1, -1, tri.v);

    //判断是否需要将边角点push
    if (is_top_right_inside)
        LerpAndPushVertex(tri, top_right_vextex, res);
    if (is_top_left_inside)
        LerpAndPushVertex(tri, top_left_vextex, res);
    if (is_bottom_right_inside)
        LerpAndPushVertex(tri, bottom_right_vextex, res);
    if (is_bottom_left_inside)
        LerpAndPushVertex(tri, bottom_left_vextex, res);

    for (int i = 0; i + 1 < indices.size(); ++i) {

        VertexData v_outside, v_inside;

        VertexData v0(tri.v[indices[i]], tri.normal[indices[i]], tri.tex_coords[indices[i]]);
        VertexData v1(tri.v[indices[i + 1]], tri.normal[indices[i + 1]], tri.tex_coords[indices[i + 1]]);

        int outcode0 = ComputeOutCode(v0.vertex.x, v0.vertex.y, v0.vertex.z, v0.vertex.w);
        int outcode1 = ComputeOutCode(v1.vertex.x, v1.vertex.y, v1.vertex.z, v1.vertex.w);

        bool is_v0_change = false;
        bool is_v1_change = false;

        while (true) {
            if ((outcode0 | outcode1) == 0) { //相或为0，都在立方体内，接受并且退出循环
                if (!isVertexPush[indices[i]].second || is_v0_change) {
                    res.push_back(v0);
                    isVertexPush[indices[i]].second = true;
                }
                if (!isVertexPush[indices[i + 1]].second || is_v1_change) {
                    res.push_back(v1);
                    isVertexPush[indices[i + 1]].second = true;
                }
                break;
                // 相与为1，都在立方体外的一侧，拒绝且退出循环，在不同外侧，并且该直线没有与正方体相交，
                //会在一次循环计算得到了一个新点与旧点在同一侧，然后退出。
            } else if (outcode0 & outcode1) {
                break;
                // TODO:没有考虑两点都在外面，且与正方体不相交的情况
            } else {
                //找出在界外的点
                if (is_top_right_inside &&
                    (v0.vertex.y > 1.0f && v1.vertex.x > 1.0f || v1.vertex.y > 1.0f && v0.vertex.x > 1.0f)) {
                    int aa = 1;
                }
                int outcodeOut = outcode0 ? outcode0 : outcode1; // outcodeOut是在外头点的编码
                v_inside = (outcode0 == outcodeOut) ? v1 : v0;
                v_outside = (outcode0 == outcodeOut) ? v0 : v1;
                float x1 = v_inside.vertex.x, y1 = v_inside.vertex.y, z1 = v_inside.vertex.z, w1 = 1; // TODO: W的值不确定是1还是1.1
                float x2 = v_outside.vertex.x, y2 = v_outside.vertex.y, z2 = v_outside.vertex.z, w2 = 1;

                VertexData crossPoint;
                // 找出和边界相交的点 //TODO: z的边界并非[0,1]
                if (outcodeOut & ClipOutCode::kTop) {
                    crossPoint.vertex.x = (w2 - y1) * (x2 - x1) / (y2 - y1) + x1;
                    crossPoint.vertex.y = w2;
                    crossPoint.vertex.z = (w2 - y1) * (z2 - z1) / (y2 - y1) + z1;
                    // crossPoint.vertex.w = w2;//不需要用到w的值
                } else if (outcodeOut & ClipOutCode::kBottom) {
                    crossPoint.vertex.x = (-w2 - y1) * (x2 - x1) / (y2 - y1) + x1;
                    crossPoint.vertex.y = -w2;
                    crossPoint.vertex.z = (-w2 - y1) * (z2 - z1) / (y2 - y1) + z1;
                    // crossPoint.vertex.w = w2;
                } else if (outcodeOut & ClipOutCode::kLeft) {
                    crossPoint.vertex.x = -w2;
                    crossPoint.vertex.y = (-w2 - x1) * (y2 - y1) / (x2 - x1) + y1;
                    crossPoint.vertex.z = (-w2 - x1) * (z2 - z1) / (x2 - x1) + z1;
                    // crossPoint.vertex.w = w2;
                } else if (outcodeOut & ClipOutCode::kRight) {
                    crossPoint.vertex.x = w2;
                    crossPoint.vertex.y = (w2 - x1) * (y2 - y1) / (x2 - x1) + y1;
                    crossPoint.vertex.z = (w2 - x1) * (z2 - z1) / (x2 - x1) + z1;
                    // crossPoint.vertex.w = w2;
                } else if (outcodeOut & ClipOutCode::kNear) {
                    crossPoint.vertex.x = (-1 - z1) * (x2 - x1) / (z2 - z1) + x1; // modify 0->-1
                    crossPoint.vertex.y = (-1 - z1) * (y2 - y1) / (z2 - z1) + y1;
                    crossPoint.vertex.z = -1;
                    // crossPoint.vertex.w = 0;
                } else if (outcodeOut & ClipOutCode::kFar) {
                    crossPoint.vertex.x = (w2 - z1) * (x2 - x1) / (z2 - z1) + x1;
                    crossPoint.vertex.y = (w2 - z1) * (y2 - y1) / (z2 - z1) + y1;
                    crossPoint.vertex.z = w2;
                    // crossPoint.vertex.w = w2;
                }
                //交叉点各属性插值
                float dy = crossPoint.vertex.y - v0.vertex.y;
                float t = dy / (v1.vertex.y - v0.vertex.y);
                crossPoint.tex = MathUtil::Lerp(v0.tex, v1.tex, t);
                crossPoint.normal = MathUtil::Lerp(v0.normal, v1.normal, t);

                // 为什么继续循环，因为另一个端点都有可能也在外面，需要将原来外面的点用算出来的交点取代，
                //然后再进行一次判断，直到accept
                if (outcodeOut == outcode0) {
                    v0 = crossPoint;
                    is_v0_change = true;
                    outcode0 = ComputeOutCode(v0.vertex.x, v0.vertex.y, v0.vertex.z, v0.vertex.w);
                } else {
                    v1 = crossPoint;
                    is_v1_change = true;
                    outcode1 = ComputeOutCode(v1.vertex.x, v1.vertex.y, v1.vertex.z, v1.vertex.w);
                }
            }
        }
    }
}

void Rasterizer::IsCohenSutherLandClip(const Triangle &tri, std::vector<Triangle> &t_out) {
    std::vector<VertexData> vecs;
    GetCrossVertex(tri, vecs);
    //对得到的顶点排序
    MathUtil::SortPoint(vecs);
    //根据新顶点数量划分三角形
    Triangle temp;
    for (int i = 1; i + 1 < vecs.size(); ++i) {
        temp.setVertexes({vecs[0].vertex, vecs[i].vertex, vecs[i + 1].vertex});
        temp.setNormals({vecs[0].normal, vecs[i].normal, vecs[i + 1].normal});
        temp.setTexCoords({vecs[0].tex, vecs[i].tex, vecs[i + 1].tex});
        t_out.push_back(temp);
    }
}

void Rasterizer::LerpAndPushVertex(const Triangle &tri, const Maths::Vector3f &v, std::vector<VertexData> &res) {
    float alpha, beta, gamma;
    std::tie(alpha, beta, gamma) = MathUtil::ComputeBarycentric2D(v.x, v.y, tri.v);
    Maths::Vector4f interpolater_vertex = MathUtil::Interpolate(alpha, beta, gamma, tri.v[0], tri.v[1], tri.v[2],
                                                                1); //法向量插值
    interpolater_vertex.x = v.x;
    interpolater_vertex.y = v.y;
    Maths::Vector3f interpolater_normal = MathUtil::Interpolate(alpha, beta, gamma, tri.normal[0], tri.normal[1],
                                                                tri.normal[2], 1); //法向量插值
    Maths::Vector2f interpolater_texcoords = MathUtil::Interpolate(alpha, beta, gamma, tri.tex_coords[0],
                                                                   tri.tex_coords[1], tri.tex_coords[2], 1); //纹理坐标插值
    res.push_back(VertexData{interpolater_vertex, interpolater_normal, interpolater_texcoords});
}

//左手坐标系
void ComputeRadianFromXPositive(const Maths::Vector3f &point) {
    Maths::Vector3f x_positive{1, 0, 0};
}

void Rasterizer::SetModel(const Maths::Matrix4f &m) {
    model = m;
}

void Rasterizer::SetView(const Maths::Matrix4f &v) {
    view = v;
}

void Rasterizer::SetProjection(const Maths::Matrix4f &p) {
    projection = p;
}

void Rasterizer::SetVertexShader(std::function<Maths::Vector3f(VertexShaderPayload)> vert_shader) {
    vertex_shader = vert_shader;
}

void Rasterizer::SetFragmentShader(std::function<Maths::Vector3f(FragmentShaderPayload)> frag_shader) {
    fragment_shader = frag_shader;
}

void Rasterizer::SetTexture(Texture2D tex) {
    texture = tex;
}

uint32_t *&Rasterizer::GetFrameBuffer() {
    return frame_image_->GetFrameBuffer();
}

void Rasterizer::InitFloor() {
    // floor 注意：三角形顺序该项目是逆时针索引，不然会着色在背面。
    float floorScale = 2.5f;
    Maths::Vector3f verts[4] = {{-1 * floorScale, -1, -1 * floorScale},
                                {1 * floorScale,  -1, -1 * floorScale},
                                {1 * floorScale,  -1, 1 * floorScale},
                                {-1 * floorScale, -1, 1 * floorScale}}; //顶点
    uint32_t vertIndex[6] = {0, 3, 2, 0, 2,
                             1}; //三角形索引
    Maths::Vector2f st[4] = {{0, 1},
                             {1, 1},
                             {1, 0},
                             {0, 0}}; //
    for (int i = 0; i < 2; ++i) {
        Triangle *triangleFloor = new Triangle();
        for (int j = 0; j < 3; j++) {
            triangleFloor->setVertex(j, Maths::ToVector4f(verts[vertIndex[i * 3 + j]], 1.0f));
            triangleFloor->setNormal(j, Maths::Vector3f(0, 1, 0));
            triangleFloor->setTexCoord(j, st[vertIndex[i * 3 + j]]);
        }
        obj_list_.floor_.push_back(triangleFloor);
    }
}
