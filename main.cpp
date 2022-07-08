#include <windows.h>
#include <windowsx.h>
#include <iostream>

#include "softRender/math/Matrix.h"
#include "softRender/math/Vector.h"
using namespace Maths;

HINSTANCE g_hInstance;  //实例句柄
static HWND g_hWnd;     //窗口句柄

static const wchar_t* const g_title = L"TinyD3D";  //窗口标题
int g_width = 800;                                 //窗口大小
int g_height = 600;

//声明函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//主函数WinMain（）
int WINAPI WinMain(_In_ HINSTANCE hInstance,
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_ LPSTR lpCmdLine,
                   _In_ int nShowCmd) {
    //窗口类名
    TCHAR cls_name[] = TEXT("MyWinClass");

    //设计窗口类
    WNDCLASSEX wce = {0};
    wce.cbClsExtra = 0;
    wce.cbSize = sizeof(wce);  //结构体大小
    wce.cbWndExtra = 0;
    wce.style = CS_HREDRAW | CS_VREDRAW;  //大小改变,水平和垂直重绘
    wce.lpfnWndProc = WindowProc;         //窗口回调函数地址
    wce.cbClsExtra = 0;                   //类的附加数据
    wce.cbWndExtra = 0;                   //窗口的附加数据
    wce.hInstance = g_hInstance;          //应用程序实例句柄
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);     //大图标
    wce.hIconSm = wce.hIcon;                         //小图标
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);       //光标
    wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  //背景色
    wce.lpszMenuName = nullptr;                      //菜单
    wce.lpszClassName = cls_name;                    //窗口类的名称

    //注册窗口类
    // ATOM nres = RegisterClassEx(&wce);
    if (FALSE == RegisterClassEx(&wce))
        return 1;

    //创建窗口
    HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW,      //窗口的扩展样式
                               cls_name,             //窗口类名
                               TEXT("MyRender"),     //窗口标题
                               WS_OVERLAPPEDWINDOW,  //窗口风格样式
                               CW_USEDEFAULT, CW_USEDEFAULT, g_width,
                               g_height,     //窗口x,y,宽度,高度
                               nullptr,      //父窗口句柄
                               nullptr,      //菜单句柄
                               g_hInstance,  //应用程序实例句柄
                               nullptr);     //附加数据

    //回调函数要写上默认的处理DefWindowProc，不然创建失败
    if (!hWnd) {
        MessageBox(nullptr, TEXT("create window failed!"), TEXT("error"),
                   MB_OK);
        return 0;
    }

    //显示，更新窗口
    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    //添加加速键表
    HACCEL hAccel = NULL;
    /*::LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1);*/

    //消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(hWnd, hAccel,
                                  &msg))  //有加速键表时用这,没有就不用
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

//窗口消息处理函数
LRESULT CALLBACK WindowProc(HWND hWnd,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // test
            Matrix<4, 4> a = Matrix<4, 4>::identity();
            std::cout << a << std::endl;

            //渲染初始化
        } break;
        case WM_PAINT: {
        } break;
        case WM_ERASEBKGND:
            return true;
            break;
        //鼠标事件
        //鼠标按下事件
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            printf("button up: x: %d u: %d\n", GET_X_LPARAM(lParam),
                   GET_Y_LPARAM(lParam));  // test
            return 0;
        //鼠标释放事件
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            printf("button down: x: %d u: %d\n", GET_X_LPARAM(lParam),
                   GET_Y_LPARAM(lParam));  // test
            return 0;
        //鼠标移动事件
        case WM_MOUSEMOVE:
            return 0;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}