#include "Camera.h"

Maths::Matrix4f Camera::GetViewMatrix() {
    Maths::Matrix4f view = Maths::Matrix4f::Identity();

    Maths::Matrix4f translate;
    translate = {{{1, 0, 0, 0},
                  {0, 1, 0, 0},
                  {0, 0, 1, 0},
                  {-position_.x, -position_.y, -position_.z, 1}}};

    view = translate * view;
    view_ = view;
    return view;
}

Maths::Matrix4f Camera::GetModelMatrix() {

    Maths::Matrix4f rotation_x(Maths::Matrix4f::Identity()), rotation_y(Maths::Matrix4f::Identity());
    if (camera_transform_.rotate_angle_.x != 0)
        rotation_x = {{{1, 0, 0, 0},
                       {0, (float)cos(camera_transform_.rotate_angle_.x), (float)sin(camera_transform_.rotate_angle_.x), 0},
                       {0, (float)(-sin(camera_transform_.rotate_angle_.x)), (float)cos(camera_transform_.rotate_angle_.x), 0},
                       {0, 0, 0, 1}}};
    if (camera_transform_.rotate_angle_.y != 0)
        rotation_y = {{{(float)cos(camera_transform_.rotate_angle_.y), 0, (float)-sin(camera_transform_.rotate_angle_.y), 0},
                       {0, 1, 0, 0},
                       {(float)(sin(camera_transform_.rotate_angle_.y)), 0, (float)cos(camera_transform_.rotate_angle_.y), 0},
                       {0, 0, 0, 1}}};

    Maths::Matrix4f scale;
    scale = {{{camera_transform_.scale_.x, 0, 0, 0},
              {0, camera_transform_.scale_.y, 0, 0},
              {0, 0, camera_transform_.scale_.z, 0},
              {0, 0, 0, 1}}};

    Maths::Matrix4f translate;
    translate = {{{1, 0, 0, 0},
                  {0, 1, 0, 0},
                  {0, 0, 1, 0},
                  {camera_transform_.translate_.x, camera_transform_.translate_.y, camera_transform_.translate_.z, 1}}};
    Maths::Matrix4f rotate = (rotation_x * rotation_y);

    model_ = translate * rotate * scale;
    return model_;
}

Maths::Matrix4f Camera::GetProjectionMatrix() {

    Maths::Matrix4f projection = Maths::Matrix4f::Identity();
    Maths::Matrix4f persp_to_ortho;
    persp_to_ortho = {{{perspective_arg_.z_near, 0, 0, 0},
                       {0, perspective_arg_.z_near, 0, 0},
                       {0, 0, perspective_arg_.z_near + perspective_arg_.z_far, 1},
                       {0, 0, -perspective_arg_.z_near * perspective_arg_.z_far, 0}}};

    float half_eye_angle_radian = (perspective_arg_.fov / 2.0 / 180.0) * MY_PI;

    float t = std::tan(half_eye_angle_radian) * (-perspective_arg_.z_near); // zNear未添加负号，图像上下颠倒。
    float r = t * perspective_arg_.aspect_radio;
    float b = -t;
    float l = -r;
    Maths::Matrix4f m_ortho_scale;
    m_ortho_scale = {{{2 / (r - l), 0, 0, 0},
                      {0, 2 / (t - b), 0, 0},
                      {0, 0, 2 / (perspective_arg_.z_near - perspective_arg_.z_far), 0},
                      {0, 0, 0, 1}}};
    Maths::Matrix4f m_ortho_trans;

    m_ortho_trans = {{{1, 0, 0, 0},
                      {0, 1, 0, 0},
                      {0, 0, 1, 0},
                      {-(r + l) / 2, -(b + t) / 2, -(perspective_arg_.z_near + perspective_arg_.z_far) / 2.0f, 1}}};
    auto test1 = m_ortho_scale * m_ortho_trans;
    auto test2 = test1 * persp_to_ortho;
    auto test3 = test2 * projection;
    projection = m_ortho_scale * m_ortho_trans * persp_to_ortho * projection;

    projection_ = projection;
    return projection;
}
Maths::Matrix4f Camera::GetRotate(Maths::Vector3f axis, float angle) {
    float rotation_radian = (angle / 180) * MY_PI;

    Maths::Matrix3f identity_matrix = Maths::Matrix3f::Identity();
    Maths::Matrix3f n_matrix;
    n_matrix = {{{0, axis.z, -axis.y},
                 {-axis.z, 0, axis.x},
                 {axis.y, -axis.x, 0}}};

    Maths::Matrix4f axi_rotate_matrix = Maths::Matrix4f::Identity();
    Maths::Matrix3f temp_mat = cos(rotation_radian) * identity_matrix +
                               (1 - cos(rotation_radian)) * Maths::VectorMulTranspose(axis) + sin(rotation_radian) * n_matrix;
    Maths::TopLeftConner3(axi_rotate_matrix, temp_mat);
    return axi_rotate_matrix;
}