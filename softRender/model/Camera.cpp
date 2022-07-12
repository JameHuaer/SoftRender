#include "Camera.h"
using namespace ModelSpace;
void Camera::SetPosition(const Maths::Vector4f &pos) {
    position = pos;
}
void Camera::SetPerspective(float f, float aspect, float near, float far) {
    perspective_arg.fov = f;
    perspective_arg.aspect_radio = aspect;
    perspective_arg.z_near = near;
    perspective_arg.z_far = far;
}
Maths::Matrix4f Camera::LookAt() {
    Maths::Vector4f z = target - position;
    z.normalize();
    Maths::Vector4f x = Maths::Cross(up, z).normalize();
    Maths::Vector4f y = Maths::Cross(z, x);
    return Maths::Matrix4f{{{x.x(), y.x(), z.x(), 0.0f},
                            {x.y(), y.y(), z.y(), 0.0f},
                            {x.z(), y.z(), z.z(), 0.0f},
                            {-(x * position), -(y * position), -(z * position), 1.0f}}};
}
Maths::Matrix4f Camera::PerspectiveFov() {
    // std::cout << tan(perspective_arg.fov * 0.5f) << std::endl;//todo delete
    float height = 1 / tan(perspective_arg.fov * 0.5f);
    Maths::Matrix4f mat;
    mat[0][0] = height / perspective_arg.aspect_radio;
    mat[1][1] = height;
    mat[2][2] = perspective_arg.z_far / (perspective_arg.z_far - perspective_arg.z_near);
    mat[2][3] = 1.0f;
    mat[3][2] = (perspective_arg.z_near * perspective_arg.z_far) / (perspective_arg.z_near - perspective_arg.z_far);
    return mat;
}
void Camera::CalculateMatrix() {
    Maths::Matrix4f view = LookAt();
    Maths::Matrix4f projection = PerspectiveFov();
    world_view_projection = world * view * projection;
    world_invert_transpose = world.Invert().Transpose();
}

Maths::Vector4f Camera::GetPosition() {
    return position;
}
void Camera::SetWorldMatrix(const Maths::Matrix4f &w) {
    world = w;
}
Maths::Matrix4f Camera::GetWorldMatrix() {
    return world;
}
void Camera::SetWVPMatrix(const Maths::Matrix4f &wvp) {
    world_view_projection = wvp;
}
Maths::Matrix4f Camera::GetWVPMatrix() {
    return world_view_projection;
}
void Camera::SetWITMatrix(const Maths::Matrix4f &wit) {
    world_invert_transpose = wit;
}
Maths::Matrix4f Camera::GetWITMatrix() {
    return world_invert_transpose;
}
PerspectiveArg Camera::GetFovAspectRadioNearFar() {
    return perspective_arg;
}
CameraControlData Camera::GetCameraControlData() {
    return camera_control;
}
void Camera::SetCameraControlData(const CameraControlData &ccd) {
    camera_control = ccd;
}
void Camera::SetTarget(const Maths::Vector4f &tar) {
    target = tar;
}
Maths::Vector4f Camera::GetTarget() {
    return target;
}