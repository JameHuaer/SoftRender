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
#include "Camera.h"
namespace ModelSpace {
#define PI 3.1415926f
class Model {
public:
    Model(int w, int h);
    ~Model();
    void Render(); //渲染
    void Update(); //更新数据
    FrameImage *&GetFrameImage();

private:
    int width;
    int height;

    //摄像机旋转
    float theta;
    float alpha;
    float radius;
    POINT last_mouse_position; //鼠标最后点击的坐标

    // Maths::Matrix4f world_view_projection;  //世界视角投影矩阵
    // Maths::Matrix4f world_matrix;           //世界矩阵
    // Maths::Matrix4f world_invert_transpose; //世界矩阵的逆的转置，用于调整法线
    FrameImage *frame_image;   //帧图片指针
    ModelDraw *model_draw;     //模型绘制
    ShaderModel *shader_model; //着色器
    MeshData mesh_data;        //网格数据
    Camera *camera;
};

}

#endif