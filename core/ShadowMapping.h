#ifndef SHADOWMAPPING_H
#define SHADOWMAPPING_H
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
    void UpdateShadowMappingDepth(const std::vector<Triangle *> &triganleList);
    void InitZBuffer();
    void Resize(int newWidth, int newHeight);
    float GetDepth(int x, int y) const;
    void SetDepth(int x, int y, float z);
    int GetIndex(int x, int y);
    bool IsInLight(const Maths::Vector4f &worldPos);

public:
    Maths::Matrix4f view_;
    Maths::Matrix4f model_;
    Maths::Matrix4f projection_;
    Maths::Matrix4f mvp_;
    float fov = 45.0f;
    float aspect_radio = 1.0f;
    float z_near = 0.1f;
    float z_far = 50.0f;
    int width = 800;
    int height = 600;
    float **z_buffer; //帧图片深度信息缓存
    // TODO: 计算摄像机像素坐标->空间坐标->光源像素坐标。
};


#endif