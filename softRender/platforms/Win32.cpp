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
        float dx = MathUtil::AngleToRadians(0.25f * static_cast<float>(x - Win32Platform::last_mouse_position.x));
        float dy = MathUtil::AngleToRadians(0.25f * static_cast<float>(y - Win32Platform::last_mouse_position.y));
        //计算鼠标没有松开前的累计弧度
        ModelSpace::CameraControlData ccd = camera->GetCameraControlData();
        ccd.theta -= dx; //上下旋转
        ccd.phi += dy;   //左右旋转
        //限制角度phi的范围在（0.1， Pi-0.1）
        ccd.phi = MathUtil::Clamp(ccd.phi, 0.1f, PI - 0.1f);
        camera->SetCameraControlData(ccd);
    } else if (btn_state & MK_RBUTTON) {
        // TODO 平移,待优化
        float dx = 0.03f * static_cast<float>(x - Win32Platform::last_mouse_position.x);
        float dy = 0.03f * static_cast<float>(y - Win32Platform::last_mouse_position.y);
        Maths::Vector4f target = camera->GetTarget();
        target.x() = target.x() - dx;
        target.y() = target.y() - dy;
        camera->SetTarget(target); 
    }
    //将当前鼠标坐标赋值给“上一次鼠标坐标”，为下一次鼠标操作提供先前值
    Win32Platform::last_mouse_position.x = x;
    Win32Platform::last_mouse_position.y = y;
}

void Win32Platform::OnScrollMove(WPARAM btn_state, float offset) {
    ModelSpace::CameraControlData ccd = camera->GetCameraControlData();
    ccd.radius -= offset ;
    ccd.radius = MathUtil::Clamp(ccd.radius, 3.0f, 300.0f);
    camera->SetCameraControlData(ccd);
}
