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
        // z_near = near;
        // z_far = far;
    }
    float fov;
    float aspect_radio;
    float z_near;
    float z_far;
};
struct CameraTransformArg {
    CameraTransformArg() {
        scale_ = Maths::Vector3f{1.0f, 1.0f,1.0f};
        rotate_angle_ = Maths::Vector3f{0.0, MathUtil::AngleToRadians(140), 0.0f};
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
        : position(pos), target(tar), up(u) {
    }
    Camera()
        : Camera(Maths::Vector4f(0.0f, 0.0f, 10.0f, 0.0f), Maths::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), Maths::Vector4f(0.0f, 1.0f, 0.0f, 0.0f)) {
    }
    ~Camera() = default;

    //视角矩阵

    Maths::Matrix4f get_projection_matrix();
    Maths::Matrix4f get_model_matrix();
    Maths::Matrix4f get_view_matrix();
    Maths::Matrix4f get_rotate(Maths::Vector3f axis, float angle);

public:
    CameraTransformArg camera_transform_;

    Maths::Vector4f position;
    Maths::Vector4f target;
    Maths::Vector4f up;

    Maths::Matrix4f view_;
    Maths::Matrix4f model_;
    Maths::Matrix4f projection_;

    PerspectiveArg perspective_arg; // fov aspect near far
};

#endif