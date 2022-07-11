#ifndef MODELDRAW_H
#define MODELDRAW_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "Camera.h"
#include "FrameImage.h"
#include "GeometryGenerator.h"
#include "ShaderBase.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
namespace ModelSpace {
//模型填充模式
enum ModelFillMode {
    kWireFrame, //线框模式
    kSolide
};

class ModelDraw {
public:
    ModelDraw(int w, int h, Camera *car, ShaderBase *sb);
    ~ModelDraw();
    void SetFillMode(ModelFillMode mode);
    void Draw(uint32_t index_count, uint32_t start_index, uint32_t start_vertex);
    MeshData &GetMeshData();

private:
    //转到CVV空间
    void CVV(VertexShadeOut &v);
    bool IsClip(const VertexShadeOut &v);
    VertexShadeOut TransformToProjection(const VertexPrimitive &v);
    void TransformToScreen(VertexShadeOut &v);
    bool IsBackFaceCulling(const VertexPrimitive &v1, const VertexPrimitive &v2, const VertexPrimitive &v3);
    void BresenhamDrawLine(int x1, int y1, int x2, int y2);
    void ScanLineFill(const VertexShadeOut &left, const VertexShadeOut &right, int y_index);
    void DrawTriangle(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3);
    void DrawTriangleTop(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3);
    void DrawTriangleBottom(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3);
    void TriangleRasterization(const VertexShadeOut &v1, const VertexShadeOut &v2, const VertexShadeOut &v3);
    int width;
    int height;

    FrameImage *frame_image; //帧图片
    ModelFillMode fill_mode;
    ShaderBase *shader;
    Maths::Matrix4f screen_transform;
    MeshData mesh_data; //网格数据

    // Maths::Vector4f camera_pos;
    Camera *camera;
};

}

#endif