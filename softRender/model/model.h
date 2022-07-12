#ifndef MODEL_H
#define MODEL_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "FrameImage.h"
#include "GeometryGenerator.h"
#include "ModelDraw.h"
#include "ShaderModel.h"
#include <windows.h>
// #include <windowsx.h>
#include "../platforms/Win32.h"
#include "Camera.h"
namespace ModelSpace {

class Model {
public:
    Model(int w, int h);
    ~Model();
    void Render(); //渲染
    void Update(); //更新数据
    uint32_t *&GetFrameBuffer();
    PlatForms::Win32Platform *&GetPlatform();
    int GetWidth();
    int GetHeight();
    void Resize(int new_width, int new_height);

private:
    ModelDraw *model_draw;     //模型绘制
    ShaderModel *shader_model; //着色器
    Camera *camera;
    PlatForms::Win32Platform *platform_;
};

}

#endif