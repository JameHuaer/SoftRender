#ifndef CAMERA_H
#define CAMERA_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
// #include "../platforms/Win32.h"
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
struct CameraControlData {
    CameraControlData() {
        theta = 1.5f * PI;
        phi = 0.4 * PI;
        radius = 5.0f;
    }
    //摄像机旋转
    float theta;
    float phi;
    float radius;
};
class Camera {
public:
    Camera(const Maths::Vector4f &pos,
           const Maths::Vector4f &tar,
           const Maths::Vector4f &u)
        : position(pos), target(tar), up(u), world(Maths::Matrix4f::Identity()) {
    }
    Camera()
        : Camera(Maths::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), Maths::Vector4f(0.0f, 0.0f, 0.0f, 1.0f), Maths::Vector4f(0.0f, 1.0f, 0.0f, 0.0f)) {
    }
    ~Camera() = default;
    //获得摄像机坐标
    void SetPosition(const Maths::Vector4f &pos);
    Maths::Vector4f GetPosition();
    void SetTarget(const Maths::Vector4f &tar);
    Maths::Vector4f GetTarget();

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
    void CalculateMatrix();
    CameraControlData GetCameraControlData();
    void SetCameraControlData(const CameraControlData &ccd);

private:
    CameraControlData camera_control;

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