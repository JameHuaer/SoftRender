#include <iostream>
#include <memory>
#include <windows.h>
#include <windowsx.h>
// #include "softRender/Render.h"
#include "softRender/math/MathUtil.h"
#include "softRender/math/Matrix.h"
#include "softRender/math/Vector.h"
#include "softRender/model/GeometryGenerator.h"
#include "softRender/model/Model.h"
#include "softRender/model/Vertex.h"
#include "softRender/platforms/Win32.h"
using namespace Maths;
// D3D坐标系，行优先
//软光栅过程：读取模型数据或者创建模型数据（包括顶点坐标、纹理坐标、法向量、颜色）->
//计算world view projection矩阵，将模型坐标转化为屏幕坐标->
//绘制（背面消除->CVV裁剪->透视除法->绘制三角形->光栅化三角形(绘制点->绘制线->三角形插值)

HINSTANCE g_hInstance; //实例句柄
static HWND g_hWnd;    //窗口句柄

static const wchar_t *const g_title = L"SoftRender"; //窗口标题
int g_width = 800;                                   //窗口大小
int g_height = 600;
bool isResizing = false;
//声明函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::shared_ptr<ModelSpace::Model> m_model = std::make_shared<ModelSpace::Model>(g_width, g_height);

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
    wce.cbSize = sizeof(wce); //结构体大小
    wce.cbWndExtra = 0;
    wce.style = CS_HREDRAW | CS_VREDRAW;            //大小改变,水平和垂直重绘
    wce.lpfnWndProc = WindowProc;                   //窗口回调函数地址
    wce.cbClsExtra = 0;                             //类的附加数据
    wce.cbWndExtra = 0;                             //窗口的附加数据
    wce.hInstance = g_hInstance;                    //应用程序实例句柄
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);    //大图标
    wce.hIconSm = wce.hIcon;                        //小图标
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);      //光标
    wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //背景色
    wce.lpszMenuName = nullptr;                     //菜单
    wce.lpszClassName = cls_name;                   //窗口类的名称

    //注册窗口类
    // ATOM nres = RegisterClassEx(&wce);
    if (FALSE == RegisterClassEx(&wce))
        return 1;

    //创建窗口
    HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW,     //窗口的扩展样式
                               cls_name,            //窗口类名
                               TEXT("SoftRender"),    //窗口标题
                               WS_OVERLAPPEDWINDOW, //窗口风格样式
                               CW_USEDEFAULT, CW_USEDEFAULT, g_width,
                               g_height,    //窗口x,y,宽度,高度
                               nullptr,     //父窗口句柄
                               nullptr,     //菜单句柄
                               g_hInstance, //应用程序实例句柄
                               nullptr);    //附加数据

    //回调函数要写上默认的处理DefWindowProc，不然创建失败
    if (!hWnd) {
        MessageBox(nullptr, TEXT("create window failed!"), TEXT("error"),
                   MB_OK);
        return 0;
    }

    //显示，更新窗口
    ShowWindow(hWnd, nShowCmd);
    // UpdateWindow(hWnd);

    //消息循环
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            m_model->Update();
            m_model->Render();
            InvalidateRect(hWnd, nullptr, true);
            UpdateWindow(hWnd);
            // todo
            // update render;
            // Render::update(hWnd);
        }
    }
    return msg.wParam;
}
//窗口消息处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    //双缓冲绘图
    static BITMAPINFO s_bitmapInfo;
    static HDC s_hdcBackbuffer; //后缓冲设备句柄
    static HBITMAP s_hBitmap;
    static HBITMAP s_hOldBitmap;
    static void *s_pData;

    switch (uMsg) {
    case WM_CREATE: {
        //模型初始化
        //初始化设备无关位图header
        BITMAPINFOHEADER bmphdr = {0};
        bmphdr.biSize = sizeof(BITMAPINFOHEADER);
        bmphdr.biWidth = g_width;
        bmphdr.biHeight = -g_height;
        bmphdr.biPlanes = 1;
        bmphdr.biBitCount = 32;
        bmphdr.biSizeImage = g_height * g_width * 4;
        //创建后缓冲区
        //先创建一个内存dc
        s_hdcBackbuffer = CreateCompatibleDC(nullptr);
        //获得前置缓冲区dc
        HDC hdc = GetDC(hWnd);

        s_hBitmap = CreateDIBSection(nullptr, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS,
                                     reinterpret_cast<void **>(&m_model->GetFrameBuffer()), nullptr, 0);

        //将bitmap装入内存dc
        s_hOldBitmap = (HBITMAP)SelectObject(s_hdcBackbuffer, s_hBitmap);
        //释放dc
        ReleaseDC(hWnd, hdc);
    } break;
    case WM_PAINT: {
        hdc = BeginPaint(hWnd, &ps);
        //把backbuffer内容传到frontbuffer
        BitBlt(ps.hdc, 0, 0, g_width, g_height, s_hdcBackbuffer, 0, 0,
               SRCCOPY);
        EndPaint(hWnd, &ps);
    } break;
    case WM_ERASEBKGND:
        return true;
    case WM_ACTIVATE:

        break;
        //鼠标事件
        //鼠标按下事件
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        //如果g_width != width && g_height != height;那么就调整渲染窗口
        if (g_width != m_model->GetWidth() && g_height != m_model->GetHeight()) {
            //模型初始化
            //初始化设备无关位图header
            BITMAPINFOHEADER bmphdr = {0};
            bmphdr.biSize = sizeof(BITMAPINFOHEADER);
            bmphdr.biWidth = g_width;
            bmphdr.biHeight = -g_height;
            bmphdr.biPlanes = 1;
            bmphdr.biBitCount = 32;
            bmphdr.biSizeImage = g_height * g_width * 4;
            // delete m_model->GetFrameBuffer();
            s_hBitmap = CreateDIBSection(nullptr, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS,
                                         reinterpret_cast<void **>(&m_model->GetFrameBuffer()), nullptr, 0);
            //将bitmap装入内存dc
            s_hOldBitmap = (HBITMAP)SelectObject(s_hdcBackbuffer, s_hBitmap);
            m_model->Resize(g_width, g_height);
        }
        m_model->GetPlatform()->OnMouseDown(wParam, hWnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
        //鼠标释放事件
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        m_model->GetPlatform()->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
        //鼠标移动事件
    case WM_MOUSEMOVE:
        m_model->GetPlatform()->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_MOUSEWHEEL:
        m_model->GetPlatform()->OnScrollMove(wParam, GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
        return 0;
    case WM_SIZE:
        // printf("button down: w: %d h: %d\n", g_width, g_height); // test
        g_width = LOWORD(lParam);
        g_height = HIWORD(lParam);
        // TODO
        //调整窗口大小，在结束调整后同步渲染。
        //目前设计为：当缩放结束后并不会立刻渲染，而是将鼠标指针移动到窗口内点击一下之后才会。
        return 0;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        SelectObject(s_hdcBackbuffer, s_hOldBitmap);
        DeleteDC(s_hdcBackbuffer);
        DeleteObject(s_hOldBitmap);
        PostQuitMessage(0);
        break;

    default:

        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}