#include "Model.h"
using namespace ModelSpace;

Model::Model(int w, int h)
    : width(w), height(h), frame_image(new FrameImage(w, h)), camera(new Camera()),
      theta(1.5f * PI), alpha(0.4 * PI), radius(5.0f) {

    camera->SetPerspective(PI / 4, w / static_cast<float>(h), 1.f, 100.f); // todo delete
    camera->GetWorldMatrix() = Maths::Matrix4f::Identity();
    shader_model = new ShaderModel(camera); //相机实例后再实例着色器

    model_draw = new ModelDraw(w, h, camera, shader_model); //模型绘制需要相机指针

    MeshData &mesh_data = model_draw->GetMeshData(); //获得网格数据引用
    //模型创建/导入
    GeometryGenerator::GetInstace()->CreateBox(2.0f, 2.0f, 2.0f, mesh_data); //创建立方体，保存数据到mesh_data
}
Model::~Model() {
    if (frame_image)
        delete frame_image;
    if (camera)
        delete camera;
    // if (m_pShader)
    //     delete m_pShader;
}
FrameImage *&Model::GetFrameImage() {
    return frame_image;
}
void Model::Update() {
    float x = radius * sinf(alpha) * cosf(theta);
    float y = radius * cosf(alpha);
    float z = radius * sinf(alpha) * sinf(theta);

    Maths::Vector4f pos(x, y, z, 1.0f);
    Maths::Vector4f target(0.0f, 0.0f, 0.0f, 1.0f);
    Maths::Vector4f up(0.0f, 1.0f, 0.0f, 0.0f);

    camera->SetPosition(pos);  //设置摄像机新的坐标位置
    camera->CalculateMatrix(); //计算world view projection 矩阵
}

void Model::Render() {
    //清空后缓冲图片
    frame_image->ClearBuffer(Maths::Vector4f(0.75f, 0.75f, 0.75f));
    model_draw->SetFillMode(kWireFrame);
    model_draw->Draw(model_draw->GetMeshData().indices.size(), 0, 0);
    //设置渲染状态
    // m_pImmediateContext->SetRenderMode(TINY3D_FILL_SOLIDE);
    //设置着色器变量
    // m_pShader->SetWorldViewProj(m_worldViewProj);
    // m_pShader->SetWorld(m_world);
    // m_pShader->SetWorldInvTranspose(m_worldInvTranspose);

    // m_pImmediateContext->DrawIndexed(m_indices.size(), 0, 0);
}