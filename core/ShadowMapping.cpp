#include "ShadowMapping.h"

#include <utility>


std::vector<Light> lights_;

ShadowMapping::ShadowMapping(std::vector<Light> ls, int w, int h) : rotate_angle_(0, 0, 0), width(w), height(h) {
    lights_ = std::move(ls);
    //model
    model_ = Maths::Matrix4f::Identity();

    // projection
    projection_ = Maths::Matrix4f::Identity();
    Maths::Matrix4f persp_to_ortho = {{{z_near, 0, 0, 0},
                                       {0, z_near, 0, 0},
                                       {0, 0, z_near + z_far, 1},
                                       {0, 0, -z_near * z_far, 0}}};

    float half_eye_angle_radian = (fov / 2.0 / 180.0) * MY_PI;

    float t = std::tan(half_eye_angle_radian) * (-z_near); // zNear未添加负号，图像上下颠倒。
    float r = t * aspect_radio;
    float b = -t;
    float l = -r;
    Maths::Matrix4f m_ortho_scale = {{{2 / (r - l), 0, 0, 0},
                                      {0, 2 / (t - b), 0, 0},
                                      {0, 0, 2 / (z_near - z_far), 0},
                                      {0, 0, 0, 1}}};
    Maths::Matrix4f m_ortho_trans = {{{1, 0, 0, 0},
                                      {0, 1, 0, 0},
                                      {0, 0, 1, 0},
                                      {-(r + l) / 2, -(b + t) / 2, -(z_near + z_far) / 2.0f, 1}}};
    projection_ = m_ortho_scale * m_ortho_trans * persp_to_ortho * projection_;
    InitZBuffer();
}

ShadowMapping::~ShadowMapping() {
    if (z_buffer)
        for (int i = 0; i < height; ++i) {
            delete[] z_buffer[i];
        }
}

void ShadowMapping::InitZBuffer() {
    z_buffer = new float *[height]; // width*height;
    for (int i = 0; i < height; ++i) {
        z_buffer[i] = new float[width];
        for (int j = 0; j < width; ++j) {
            z_buffer[i][j] = FLOAT_MAX;
        }
    }
}

//shadow mapping 可以不用resize
void ShadowMapping::Resize(int newWight, int newHeight) {
    //宽高resize
    //创建一个新的二维数组
    float **new_z_buffer = new float *[newHeight];
    for (int i = 0; i < newHeight; ++i) {
        new_z_buffer[i] = new float[newWight];
        for (int j = 0; j < newWight; ++j) {
            new_z_buffer[i][j] = FLOAT_MAX;
        }
    }
    //将原本数据拷贝进新数组，中心对齐
    //拷贝交集部分
    int row_start = std::abs(newHeight - height) / 2;
    int col_start = std::abs(newWight - width) / 2;
    int row_max = std::fmin(newHeight, height) + row_start;
    int col_max = std::fmin(newWight, width) + col_start;
    for (int i = row_start; i < row_max; ++i) {
        for (int j = col_start; j < col_max; ++j) {
            if (newWight <= width) {
                if (newHeight <= height)
                    new_z_buffer[i - row_start][j - col_start] = z_buffer[i][j]; // 1
                else
                    new_z_buffer[i][j - col_start] = z_buffer[i - row_start][j]; // 2
            } else {
                if (newHeight <= height)
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
    width = newWight;
    height = newHeight;
}

float ShadowMapping::GetDepth(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return z_buffer[y][x];
    return 1.0f;
}

void ShadowMapping::SetDepth(int x, int y, float z) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        z_buffer[y][x] = z;
}

int ShadowMapping::GetIndex(int x, int y) {
    return (height - y - 1) * width + x - 1;
}

bool ShadowMapping::IsInLight(const Maths::Vector4f &worldPos, const Maths::Vector3f &Normal) {
    float f1 = (z_far - z_near) / 2.0f; // 24.95
    float f2 = (z_far + z_near) / 2.0f; // 25.05
    Maths::Vector4f tempPos = mvp_ * worldPos;
    float invW = 1 / tempPos.w;
    tempPos.x *= invW;
    tempPos.y *= invW;
    tempPos.z *= invW;
    tempPos.x = 0.5f * width * (tempPos.x + 1.0);
    tempPos.y = 0.5f * height * (tempPos.y + 1.0);
    tempPos.z = tempPos.z * f1 + f2;
    float lightDepth = GetDepth(tempPos.x, tempPos.y);
    float bias = std::fmax(0.05f * (1.0f - Normal.Dot(lights_[0].position)), 0.001f);
    return tempPos.z <= lightDepth + bias;
}

void ShadowMapping::UpdateShadowMappingDepth(const ObjList &obj_list) {
    UpdateTrianglesDepth(obj_list.floor_);
    UpdateTrianglesDepth(obj_list.main_obj_);
}

void ShadowMapping::UpdateTrianglesDepth(const std::vector<Triangle *> &triganleList) {
    float f1 = (z_far - z_near) / 2.0f; // 24.95
    float f2 = (z_far + z_near) / 2.0f; // 25.05
    Maths::Matrix4f mvp = mvp_;;
    Maths::Matrix4f mv = view_ * model_;;
    for (const auto &t: triganleList) {
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
        // Viewport transformation 视口变换
        for (auto &vertex: newtri.v) {
            vertex.x = 0.5f * width * (vertex.x + 1.0);
            vertex.y = 0.5f * height * (vertex.y + 1.0);
            vertex.z = vertex.z * f1 + f2;
        }
        auto v = newtri.toVector4(); //所有顶点的w值都设为1
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
                int id = GetIndex(x, y);
                //上下左右越界的不进行计算
                if (id < 0 || id > width * height || x >= width || x < 0 || y >= height || y < 0)
                    continue;
                if (MathUtil::InsideTriangle(x, y, newtri.v)) {
                    std::tie(alpha, beta, gamma) = MathUtil::ComputeBarycentric2D(x + 0.5, y + 0.5, newtri.v);
                    float w_reciprocal = 1.0f / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                    float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                    z_interpolated *= w_reciprocal;
                    if (z_interpolated < GetDepth(x, y))
                        SetDepth(x, y, z_interpolated);
                }
            }
        }
    }
}

void ShadowMapping::UpdateMVPMatrix() {
    Maths::Matrix4f rotation_x(Maths::Matrix4f::Identity()), rotation_y(Maths::Matrix4f::Identity());
    if (rotate_angle_.x != 0)
        rotation_x = {{{1, 0, 0, 0},
                       {0, (float) cosf(rotate_angle_.x),
                        (float) sinf(rotate_angle_.x), 0},
                       {0, (float) (-sinf(rotate_angle_.x)),
                        (float) cosf(rotate_angle_.x), 0},
                       {0, 0, 0, 1}}};
    if (rotate_angle_.y != 0)
        rotation_y = {
                {{(float) cosf(rotate_angle_.y), 0, (float) -sinf(rotate_angle_.y),
                  0},
                 {0, 1, 0, 0},
                 {(float) (sinf(rotate_angle_.y)), 0, (float) cosf(rotate_angle_.y),
                  0},
                 {0, 0, 0, 1}}};
    Maths::Matrix4f rotate = (rotation_x * rotation_y);
    // view
    lights_[0].position = (rotate * ToVector4f(lights_[0].position, 1)).head3();
    view_ = LookAt(lights_[0].position, Maths::Vector3f{0, 0, 0}, Maths::Vector3f{0, 1, 0});

    // mvp
    mvp_ = projection_ * view_ * model_;
}

void ShadowMapping::ClearZBuffer() {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            z_buffer[y][x] = FLOAT_MAX;
        }
    }
}
