#ifndef CAMERA_H
#define CAMERA_H
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include <iostream>
namespace ModelSpace {
struct PerspectiveArg {
    PerspectiveArg() = default;
    PerspectiveArg(float f, float a, float zn, float zf) {
        fov = f;
        aspect_radio = a;
        z_near = zn;
        z_far = zf;
        // z_near = near;
        // z_far = far;
    }
    float fov;
    float aspect_radio;
    float z_near;
    float z_far;
};
class Camera {
public:
    Camera(const Maths::Vector4f &pos,
           const Maths::Vector4f &tar,
           const Maths::Vector4f &u)
        : position(pos), target(tar), up(u), world(Maths::Matrix4f::Identity()) {
    }
    Camera()
        : target(0.0f, 0.0f, 0.0f, 1.0f), up(0.0f, 1.0f, 0.0f, 0.0f) {
        world = Maths::Matrix4f::Identity();
    }
    ~Camera() = default;
    //获得摄像机坐标
    void SetPosition(const Maths::Vector4f &pos);
    Maths::Vector4f GetPosition();
    //获得世界矩阵
    void SetWorldMatrix(const Maths::Matrix4f &w);
    Maths::Matrix4f GetWorldMatrix();
    //获得WVP变换矩阵
    void SetWVPMatrix(const Maths::Matrix4f &wvp);
    Maths::Matrix4f GetWVPMatrix();
    //获得WIT变换矩阵
    void SetWITMatrix(const Maths::Matrix4f &wit);
    Maths::Matrix4f GetWITMatrix();
    //设置透视参数
    void SetPerspective(float f, float aspect, float near, float far);
    //获得透视参数
    PerspectiveArg GetFovAspectRadioNearFar();
    //视角矩阵
    Maths::Matrix4f LookAt();         // view
    Maths::Matrix4f PerspectiveFov(); // projection
    void CalculateMatrix();           // todo

private:
    Maths::Vector4f position;
    Maths::Vector4f target;
    Maths::Vector4f up;

    Maths::Matrix4f world;
    Maths::Matrix4f world_view_projection;
    Maths::Matrix4f world_invert_transpose;

    PerspectiveArg perspective_arg; // fov aspect near far
};

}

#endif