#ifndef MODEL_H
#define MODEL_H
#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "FrameImage.h"
class Model {
   public:
    Model();
    ~Model();
    void init(int w, int h);
    void Render();
    FrameImage*& GetFrameImage() { return frame_image; }

   private:
    FrameImage* frame_image;
};
void Model::init(int w, int h) {
    frame_image = new FrameImage(w, h);
}
void Model::Render() {
    //清空后缓冲图片
    frame_image->ClearBuffer(Maths::Vector4f(192, 192, 192));

    //设置渲染状态
    // m_pImmediateContext->SetRenderMode(TINY3D_FILL_SOLIDE);
    //设置着色器变量
    // m_pShader->SetWorldViewProj(m_worldViewProj);
    // m_pShader->SetWorld(m_world);
    // m_pShader->SetWorldInvTranspose(m_worldInvTranspose);

    // m_pImmediateContext->DrawIndexed(m_indices.size(), 0, 0);
}
Model::Model() {}
Model::~Model() {
    if (frame_image)
        delete frame_image;
}
#endif