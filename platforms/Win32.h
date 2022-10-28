#ifndef WIN32_H
#define WIN32_H

#include "../core/Camera.h"
#include "../core/ShadowMapping.h"
#include "../core/Shader.h"
#include "../core/Global.h"
#include <memory>
#include <windows.h>

//enum ModelFillMode {
//    kWireFrame, //线框模式
//    kSolide
//};
namespace PlatForms {
    class PlatFormsBase {
    public:
        PlatFormsBase() = default;

        ~PlatFormsBase() = default;
    };

    class Win32Platform : PlatFormsBase {
    public:
        Win32Platform(Camera *car, std::function<Maths::Vector3f(FragmentShaderPayload)> &fs, std::optional<Texture2D> &t, std::optional<Texture2D> &nt,
                      std::optional<Texture2D> &bt, ModelFillMode &fm)
                : camera(car), fragment_shader(fs), texture(t), normalTexture(nt), bumpTexture(bt), fillMode(fm) {
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
        std::function<Maths::Vector3f(FragmentShaderPayload)> &fragment_shader;
        std::optional<Texture2D> &texture;
        std::optional<Texture2D> &normalTexture;
        std::optional<Texture2D> &bumpTexture;
        ModelFillMode &fillMode;
    };
}

#endif