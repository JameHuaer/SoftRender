#ifndef WIN32_H
#define WIN32_H

#include "../core/Camera.h"
#include "../core/ShadowMapping.h"
#include <memory>
#include <windows.h>

namespace PlatForms {
    class PlatFormsBase {
    public:
        PlatFormsBase() = default;

        ~PlatFormsBase() = default;
    };

    class Win32Platform : PlatFormsBase {
    public:
        Win32Platform(Camera *car)
                : camera(car) {
        }

        void SetShadowMapping(ShadowMapping *sm);

    public:
        //鼠标按下
        void OnMouseDown(WPARAM btn_state, HWND hWnd, int x, int y);

        //鼠标抬起
        void OnMouseUp(WPARAM btn_state, int x, int y);

        //鼠标移动
        void OnMouseMove(WPARAM btn_state, int x, int y);

        //滚轮滑动
        void OnScrollMove(WPARAM btn_state, float offset);

        //方向键
        void OnKeyBoardDown(WPARAM btn_state, bool isPressed, HWND hWnd);

    public:
        ShadowMapping *shadowMapping_;
    private:
        POINT last_mouse_position;
        Camera *camera;
    };
}

#endif