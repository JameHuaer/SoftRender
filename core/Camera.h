#ifndef CAMERA_H
#define CAMERA_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
// #include "../platforms/Win32.h"
#include "Global.h"
#include <iostream>
struct PerspectiveArg {
    PerspectiveArg() = default;
    PerspectiveArg(float f, float a, float zn, float zf) {
        fov = f;
        aspect_radio = a;
        z_near = zn;
        z_far = zf;
    }
    float fov;
    float aspect_radio;
    float z_near;
    float z_far;
};
struct CameraTransformArg {
    CameraTransformArg() {
        scale_ = Maths::Vector3f{2.5f, 2.5f, 2.5f};
        rotate_angle_ = Maths::Vector3f{0.0, MathUtil::AngleToRadians(320), 0.0f};
        translate_ = Maths::Vector3f{0.0, 0.0, 0.0};
    }
    Maths::Vector3f rotate_angle_;
    Maths::Vector3f scale_;
    Maths::Vector3f translate_;
};

class Camera {
public:
    Camera(const Maths::Vector4f &pos,
           const Maths::Vector4f &tar,
           const Maths::Vector4f &u)
        : position_(pos), target_(tar), up_(u) {
    }
    Camera()
        : Camera(Maths::Vector4f(0.0f, 0.0f, 20.0f, 0.0f), Maths::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), Maths::Vector4f(0.0f, 1.0f, 0.0f, 0.0f)) {
    }
    ~Camera() = default;

    //视角矩阵

    Maths::Matrix4f GetProjectionMatrix();
    Maths::Matrix4f GetModelMatrix();
    Maths::Matrix4f GetViewMatrix();
    Maths::Matrix4f GetRotate(Maths::Vector3f axis, float angle);

public:
    CameraTransformArg camera_transform_;

    Maths::Vector4f position_;
    Maths::Vector4f target_;
    Maths::Vector4f up_;

    Maths::Matrix4f view_;
    Maths::Matrix4f model_;
    Maths::Matrix4f projection_;

    PerspectiveArg perspective_arg_; // fov aspect near far
};

#endif