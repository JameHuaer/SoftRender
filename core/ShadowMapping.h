#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "RenderUtil.h"
#include "Triangle.h"
#define FLOAT_MAX 3.402823E38
class ShadowMapping {
private:
public:
    ShadowMapping(Maths::Vector3f position, Maths::Vector3f intensity);
    ~ShadowMapping();
    void UpdateShadowMappingDepth(std::vector<Triangle *> &triganleList);
    void InitZBuffer();
    void Resize(int newWidth, int newHeight);
    float GetDepth(int x, int y) const;
    void SetDepth(int x, int y, float z);
    int GetIndex(int x, int y);

public:
    Maths::Matrix4f view_;
    Maths::Matrix4f model_;
    Maths::Matrix4f projection_;
    Maths::Matrix4f mvp_;
    float fov = 45.0f;
    float aspect_radio = 1.0f;
    float z_near = 0.1f;
    float z_far = 50.0f;
    float width = 800;
    float height = 600;
    float **z_buffer; //帧图片深度信息缓存
    //TODO: 计算摄像机像素坐标->空间坐标->光源像素坐标。
};

ShadowMapping::ShadowMapping(Maths::Vector3f position, Maths::Vector3f intensity) {
    // model
    model_ = Maths::Matrix4f::Identity();
    // view
    Maths::Matrix4f view_ = Maths::Matrix4f::Identity();

    Maths::Matrix4f translate;
    translate = {{{1, 0, 0, 0},
                  {0, 1, 0, 0},
                  {0, 0, 1, 0},
                  {-position.x, -position.y, -position.z, 1}}};

    view_ = translate * view_;
    // projection
    Maths::Matrix4f projection_ = Maths::Matrix4f::Identity();
    Maths::Matrix4f persp_to_ortho;
    persp_to_ortho = {{{z_near, 0, 0, 0},
                       {0, z_near, 0, 0},
                       {0, 0, z_near + z_far, 1},
                       {0, 0, -z_near * z_far, 0}}};

    float half_eye_angle_radian = (fov / 2.0 / 180.0) * MY_PI;

    float t = std::tan(half_eye_angle_radian) * (-z_near); // zNear未添加负号，图像上下颠倒。
    float r = t * aspect_radio;
    float b = -t;
    float l = -r;
    Maths::Matrix4f m_ortho_scale;
    m_ortho_scale = {{{2 / (r - l), 0, 0, 0},
                      {0, 2 / (t - b), 0, 0},
                      {0, 0, 2 / (z_near - z_far), 0},
                      {0, 0, 0, 1}}};
    Maths::Matrix4f m_ortho_trans;

    m_ortho_trans = {{{1, 0, 0, 0},
                      {0, 1, 0, 0},
                      {0, 0, 1, 0},
                      {-(r + l) / 2, -(b + t) / 2, -(z_near + z_far) / 2.0f, 1}}};
    projection_ = m_ortho_scale * m_ortho_trans * persp_to_ortho * projection_;
    // mvp
    mvp_ = projection_ * view_ * model_;
    InitZBuffer();
}

ShadowMapping::~ShadowMapping() {
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
    // return width * y + x;
}
void ShadowMapping::UpdateShadowMappingDepth(std::vector<Triangle *> &triganleList) {
    float f1 = (z_far - z_near) / 2.0; // 24.95
    float f2 = (z_far + z_near) / 2.0; // 25.05
    Maths::Matrix4f mvp = projection_ * view_ * model_;

    for (const auto &t : triganleList) {
        Triangle newtri = *t;

        //计算视口矩阵
        std::array<Maths::Vector4f, 3> mm{
            (view_ * model_ * t->v[0]),
            (view_ * model_ * t->v[1]),
            (view_ * model_ * t->v[2])};
        //保存视口坐标
        std::array<Maths::Vector3f, 3> viewspace_pos;
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
        for (int i = 0; i < 3; ++i) {
            newtri.v[i].x /= newtri.v[i].w;
            newtri.v[i].y /= newtri.v[i].w;
            newtri.v[i].z /= newtri.v[i].w;
        }
        //法线
        Maths::Matrix4f inv_trans = (view_ * model_).Invert().Transpose();
        Maths::Vector4f n[] = {
            inv_trans * Maths::ToVector4f(newtri.normal[0], 0.0f),
            inv_trans * Maths::ToVector4f(newtri.normal[1], 0.0f),
            inv_trans * Maths::ToVector4f(newtri.normal[2], 0.0f)};
        // Viewport transformation 视口变换
        for (int j = 0; j < 3; ++j) {
            newtri.v[j].x = 0.5 * width * (newtri.v[j].x + 1.0);
            newtri.v[j].y = 0.5 * height * (newtri.v[j].y + 1.0);
            newtri.v[j].z = newtri.v[j].z * f1 + f2;
        }
        // view space normal
        newtri.setNormals({n[0].head3(), n[1].head3(), n[2].head3()}); //向量缩小到3维

        newtri.setColor(0, 148, 121.0, 92.0);
        newtri.setColor(1, 148, 121.0, 92.0);
        newtri.setColor(2, 148, 121.0, 92.0);

        auto v = newtri.toVector4(); //所有顶点的w值都设为1
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
                int id = GetIndex(x, y);
                //上下左右越界的不进行着色，缩放超出屏幕也不进行着色，在进行多边形裁剪后，该语句没有必要
                if (id < 0 || id > width * height || x >= width || x < 0 || y >= height || y < 0)
                    continue;
                if (MathUtil::InsideTriangle(x, y, newtri.v)) {
                    std::tie(alpha, beta, gamma) = MathUtil::ComputeBarycentric2D(x + 0.5, y + 0.5, newtri.v);
                    float w_reciprocal = 1.0 / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                    float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                    z_interpolated *= w_reciprocal;
                    if (z_interpolated < GetDepth(x, y))
                        SetDepth(x, y, z_interpolated);
                }
            }
        }
    }
}
