#include "ModelDraw.h"
using namespace ModelSpace;
ModelDraw::ModelDraw(int w, int h, Camera *car, ShaderBase *sb)
    : fill_mode(kWireFrame), width(w), height(h), camera(car), shader(sb), frame_image(new FrameImage(w, h)) {
    screen_transform = MathUtil::ScreenTransformMatrix(width, height);
}
ModelDraw::~ModelDraw() {
    if (frame_image)
        delete frame_image;
}
void ModelDraw::SetFillMode(ModelFillMode mode) {
    fill_mode = mode;
}

void ModelDraw::Draw(uint32_t index_count, uint32_t start_index, uint32_t start_vertex) {
    for (int i = start_index; i < index_count / 3; ++i) {
        VertexPrimitive p1 = mesh_data.vertices[start_vertex + mesh_data.indices[3 * i]];
        VertexPrimitive p2 = mesh_data.vertices[start_vertex + mesh_data.indices[3 * i + 1]];
        VertexPrimitive p3 = mesh_data.vertices[start_vertex + mesh_data.indices[3 * i + 2]];
        //背面消隐
        if (IsBackFaceCulling(p1, p2, p3))
            continue;
        //转换到齐次裁剪空间，即投影后的坐标
        VertexShadeOut v1 = TransformToProjection(p1);
        VertexShadeOut v2 = TransformToProjection(p2);
        VertexShadeOut v3 = TransformToProjection(p3);
        //判断是否通过cvv裁剪
        if (IsClip(v1) && IsClip(v2) && IsClip(v3))
            continue;
        //进行透视除法 转到cvv
        CVV(v1);
        CVV(v2);
        CVV(v3);
        //将投影得到的坐标转化为屏幕坐标
        TransformToScreen(v1);
        TransformToScreen(v2);
        TransformToScreen(v3);
        DrawTriangle(v1, v2, v3);
    }
}
MeshData &ModelDraw::GetMeshData() {
    return mesh_data;
}
//转化到cvv空间
void ModelDraw::CVV(VertexShadeOut &v) {
    v.view_projection = v.view_projection / v.view_projection.w();
    v.view_projection.w() = 1.0f;
}
//简单cvv裁剪
bool ModelDraw::IsClip(const VertexShadeOut &v) {
    //如果abs(x)>abs(w)则x一定超出[-1,1]^3空间，三个坐标都不在CVV裁剪空间，就不进行裁剪
    if (abs(v.view_projection.x()) <= abs(v.view_projection.w()) &&
        abs(v.view_projection.y()) <= abs(v.view_projection.w()) &&
        v.view_projection.z() >= 0.0f && v.view_projection.z() <= v.view_projection.w())
        return false;
    return true;
}
//转到齐次裁剪空间
VertexShadeOut ModelDraw::TransformToProjection(const VertexPrimitive &v) {
    VertexShadeOut out = shader->VertexShader(v);
    //设置oneDivZ
    out.one_div_z = 1 / out.view_projection.w();
    //由于1/z和x,y成线性关系
    //这里将需要插值的信息都乘以1/z 得到 s/z和t/z等，方便光栅化阶段进行插值
    out.color.x() *= out.one_div_z;
    out.color.y() *= out.one_div_z;
    out.color.z() *= out.one_div_z;
    //TODO 待修改
    out.normal.x() *= out.one_div_z;    
    out.normal.y() *= out.one_div_z;
    out.normal.z() *= out.one_div_z;

    out.texcoord.x() *= out.one_div_z;
    out.texcoord.y() *= out.one_div_z;

    return out;
}
//转换到屏幕坐标
void ModelDraw::TransformToScreen(VertexShadeOut &v) {
    v.view_projection = v.view_projection * screen_transform;
}
//背面消隐
bool ModelDraw::IsBackFaceCulling(const VertexPrimitive &v1, const VertexPrimitive &v2, const VertexPrimitive &v3) {
    if (fill_mode == kWireFrame)
        return false;
    Maths::Vector4f vec1 = v2.position - v1.position;
    Maths::Vector4f vec2 = v3.position - v2.position;
    //顶点缓存中顺序为顺时针
    //叉积得到的方向与右手系一致
    Maths::Vector4f normal = Maths::Cross(vec1, vec2);
    Maths::Vector4f viewDirection = v1.position - camera->GetPosition();
    if (normal * viewDirection < 0)
        return false;
    return true;
}
// Bresenham画线
void ModelDraw::BresenhamDrawLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int stepx = 1;
    int stepy = 1;

    if (dx >= 0) {
        stepx = 1;
    } else {
        stepx = -1;
        dx = abs(dx);
    }

    if (dy >= 0) {
        stepy = 1;
    } else {
        stepy = -1;
        dy = abs(dy);
    }

    int deltaX = 2 * dx;
    int deltaY = 2 * dy;
    if (dx > dy) {
        int error = deltaY - dx;
        for (int i = 0; i <= dx; ++i) {
            if (x1 >= 0 && x1 < frame_image->GetWidth() && y1 >= 0 && y1 < frame_image->GetHeight())
                frame_image->DrawPixel(x1, y1, Maths::Vector4f(0.f, 0.f, 0.f, 1.f));
            if (error >= 0) {
                error -= deltaX;
                y1 += stepy;
            }
            error += deltaY;
            x1 += stepx;
        }
    } else {
        int error = deltaX - dy;
        for (int i = 0; i <= dy; i++) {
            if (x1 >= 0 && x1 < frame_image->GetWidth() && y1 >= 0 && y1 < frame_image->GetHeight())
                frame_image->DrawPixel(x1, y1, Maths::Vector4f(0.f, 0.f, 0.f, 1.f));
            if (error >= 0) {
                error -= deltaY;
                x1 += stepx;
            }
            error += deltaX;
            y1 += stepy;
        }
    }
}
void ModelDraw::ScanLineFill(const VertexShadeOut &left, const VertexShadeOut &right, int y_index) {
    float dx = right.view_projection.x() - left.view_projection.x();
    for (float x = left.view_projection.x(); x <= right.view_projection.x(); x += 1.0f) {
        int x_index = static_cast<int>(x + 0.5f);

        if (x_index >= 0 && x_index < width) {
            //插值系数
            float lerp_factor = 0;
            if (dx != 0)
                lerp_factor = (x - left.view_projection.x()) / dx;
            //深度测试
            // 1/z’与x’和y'是线性关系的
            float odz = MathUtil::Lerp(left.one_div_z, right.one_div_z, lerp_factor);
            if (odz >= frame_image->GetDepth(x_index, y_index)) {
                frame_image->SetDepth(x_index, y_index, odz);

                float w = 1 / odz;
                //插值顶点 原先需要插值的信息均乘以oneDivZ
                //现在得到插值后的信息需要除以oneDivZ得到真实值
                VertexShadeOut out = MathUtil::Lerp(left, right, lerp_factor);
                out.view_projection.y() = y_index;
                out.texcoord = out.texcoord * w;
                out.normal.x() *= w;
                out.normal.y() *= w;
                out.normal.z() *= w;
                out.color.x() *= w;
                out.color.y() *= w;
                out.color.z() *= w;
                frame_image->DrawPixel(x_index, y_index, shader->PixelShader(out));
            }
        }
    }
}
void ModelDraw::DrawTriangle(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3) {
    switch (fill_mode) {
    case kWireFrame:
        BresenhamDrawLine(v1.view_projection.x(), v1.view_projection.y(), v2.view_projection.x(), v2.view_projection.y());
        BresenhamDrawLine(v1.view_projection.x(), v1.view_projection.y(), v3.view_projection.x(), v3.view_projection.y());
        BresenhamDrawLine(v2.view_projection.x(), v2.view_projection.y(), v3.view_projection.x(), v3.view_projection.y());
        break;
    case kSolide:
        TriangleRasterization(v1, v2, v3);
        break;
    default:
        break;
    }
}
void ModelDraw::DrawTriangleTop(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3) {
    float dy = 0;
    for (float y = v1.view_projection.y(); y <= v3.view_projection.y(); y += 1.0f) {

        int y_index = static_cast<int>(y + 0.5f);
        if (y_index >= 0 && y_index < height) {
            float t = dy / (v3.view_projection.y() - v1.view_projection.y());
            //插值生成左右顶点
            VertexShadeOut left = MathUtil::Lerp(v1, v3, t);
            VertexShadeOut right = MathUtil::Lerp(v2, v3, t);
            dy += 1.0f;
            if (left.view_projection.x() < right.view_projection.x())
                ScanLineFill(left, right, y_index);
            else
                ScanLineFill(right, left, y_index);
        }
    }
}
void ModelDraw::DrawTriangleBottom(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3) {
}
//光栅化三角形
void ModelDraw::TriangleRasterization(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3) {
    std::vector<VertexShadeOut> vers{v1, v2, v3};
    //根据y值将三个顶点排序
    std::sort(vers.begin(), vers.end(),
              [](const VertexShadeOut &v1, const VertexShadeOut &v2) { return v1.view_projection.y() < v2.view_projection.y(); });
    VertexShadeOut &top = vers[0];
    VertexShadeOut &middle = vers[1];
    VertexShadeOut &bottom = vers[2];
    //插值求中间点
    float middle_x = (middle.view_projection.y() - top.view_projection.y()) * (bottom.view_projection.x() - top.view_projection.x()) /
                         (bottom.view_projection.y() - top.view_projection.y()) +
                     top.view_projection.x();
    float dy = middle.view_projection.y() - top.view_projection.y();
    float t = dy / (bottom.view_projection.y() - top.view_projection.y());

    VertexShadeOut lerp_middle = MathUtil::Lerp(top, bottom, t);
    lerp_middle.view_projection.x() = middle_x;
    lerp_middle.view_projection.y() = middle.view_projection.y();

    DrawTriangleTop(middle, lerp_middle, bottom);
    DrawTriangleBottom(top, middle, lerp_middle);
}