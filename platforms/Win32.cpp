#include "Win32.h"

using namespace PlatForms;

void Win32Platform::OnMouseDown(WPARAM btn_state, HWND hWnd, int x, int y) {
    Win32Platform::last_mouse_position.x = x; //记录按下的坐标值
    Win32Platform::last_mouse_position.y = y;
    SetCapture(hWnd); //在属于当前线程的指定窗口里，设置鼠标捕获
}

void Win32Platform::OnMouseUp(WPARAM btn_state, int x, int y) {
    ReleaseCapture(); //按键抬起后释放鼠标捕获
}

void Win32Platform::OnMouseMove(WPARAM btn_state, int x, int y) {
    //是否是鼠标左键按下移动鼠标
    if (btn_state & MK_LBUTTON) { //旋转 鼠标左键
        //将鼠标的移动距离换算成弧度，0.25为调节阈值
        float dx = MathUtil::AngleToRadians(0.6f * static_cast<float>(x - Win32Platform::last_mouse_position.x));
        float dy = MathUtil::AngleToRadians(0.6f * static_cast<float>(y - Win32Platform::last_mouse_position.y));
        //计算鼠标没有松开前的累计弧度
        camera->camera_transform_.rotate_angle_.x += dy;
        camera->camera_transform_.rotate_angle_.y += dx;

    } else if (btn_state & MK_RBUTTON) {
        float dx = 0.01f * static_cast<float>(x - Win32Platform::last_mouse_position.x);
        float dy = 0.01f * static_cast<float>(y - Win32Platform::last_mouse_position.y);
        camera->camera_transform_.translate_.x += dx;
        camera->camera_transform_.translate_.y -= dy;
    }
    //将当前鼠标坐标赋值给“上一次鼠标坐标”，为下一次鼠标操作提供先前值
    Win32Platform::last_mouse_position.x = x;
    Win32Platform::last_mouse_position.y = y;
}

void Win32Platform::OnScrollMove(WPARAM btn_state, float offset) {
    float zoom_in = 1.2f;
    float zoom_out = 0.8f;
    if (offset < 0) {
        camera->camera_transform_.scale_.x *= zoom_out;
        camera->camera_transform_.scale_.y *= zoom_out;
        camera->camera_transform_.scale_.z *= zoom_out;
    } else {
        camera->camera_transform_.scale_.x *= zoom_in;
        camera->camera_transform_.scale_.y *= zoom_in;
        camera->camera_transform_.scale_.z *= zoom_in;
    }
}

//right: 39, left: 37,down: 38, up: 40,
void Win32Platform::OnKeyBoardDown(WPARAM btn_state, bool isPressed, HWND hWnd) {
    float dx = 0;
    float dy = 0;
    float delta = 0.01f;
    switch (btn_state) {
        case VK_LEFT:
            dy = -delta;
            break;
        case VK_DOWN:
            dx = delta;
            break;
        case VK_RIGHT:
            dy = delta;
            break;
        case VK_UP:
            dx = -delta;
            break;
        case VK_SPACE:
            shadowMapping_->rotate_angle_.x = 0;
            shadowMapping_->rotate_angle_.y = 0;
            break;
        case VK_F1:
            texture = normalTexture;
            fragment_shader = NormalFragmentShader;
            break;
        case VK_F2:
            texture = normalTexture;
            fragment_shader = TextureFragmentShader;
            break;
        case VK_F3:
            texture = normalTexture;
            fragment_shader = PhongFragmentShader;
            break;
        case VK_F4:
            texture = bumpTexture;
            fragment_shader = DisplacementFragmentShader;
            break;
        case VK_F5:
            texture = bumpTexture;
            fragment_shader = BumpFragmentShader;
            break;
        case VK_F6:
            fillMode = fillMode == ModelFillMode::kSolide ? ModelFillMode::kWireFrame : ModelFillMode::kSolide;
            break;
        default:
            break;
    }
    shadowMapping_->rotate_angle_.x += dx;
    shadowMapping_->rotate_angle_.y += dy;
}

void Win32Platform::SetShadowMapping(ShadowMapping *sm) {
    shadowMapping_ = sm;
}

