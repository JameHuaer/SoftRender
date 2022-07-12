#include "Model.h"
using namespace ModelSpace;

Model::Model(int w, int h)
    : camera(new Camera()) {

    camera->SetPerspective(PI / 4, w / static_cast<float>(h), 1.0f, 100.f);
    camera->SetWorldMatrix(Maths::Matrix4f::Identity());

    platform_ = new PlatForms::Win32Platform(camera);

    shader_model = new ShaderModel(camera); //相机实例后再实例着色器

    model_draw = new ModelDraw(w, h, camera, shader_model); //模型绘制需要相机指针

    MeshData &mesh_data = model_draw->GetMeshData(); //获得网格数据引用
    //模型创建/导入
    GeometryGenerator::GetInstace()->CreateBox(2.0f, 2.0f, 2.0f, mesh_data); //创建立方体，保存数据到mesh_data
}
void Model::Resize(int new_width, int new_height) {
    camera->SetPerspective(PI / 4, new_width / static_cast<float>(new_height), 1.0f, 100.f);
    model_draw->Resize(new_width, new_height);
}
Model::~Model() {
    if (camera)
        delete camera;
    if (shader_model)
        delete shader_model;
    if (model_draw)
        delete model_draw;
    if (platform_)
        delete platform_;
}

uint32_t *&Model::GetFrameBuffer() {
    return model_draw->GetFrameImage()->GetFrameBuffer();
}
void Model::Update() {
    CameraControlData camera_control = camera->GetCameraControlData();
    float x = camera_control.radius * sinf(camera_control.phi) * cosf(camera_control.theta);
    float y = camera_control.radius * cosf(camera_control.phi);
    float z = camera_control.radius * sinf(camera_control.phi) * sinf(camera_control.theta);

    Maths::Vector4f pos(x, y, z, 1.0f);

    camera->SetPosition(pos);  //设置摄像机新的坐标位置
    camera->CalculateMatrix(); //计算world view projection 矩阵
}

void Model::Render() {
    //清空后缓冲图片
    model_draw->GetFrameImage()->ClearBuffer(FrameImage::kGray);
    //设置填充模式
    model_draw->SetFillMode(kWireFrame);
    //绘制
    model_draw->Draw(model_draw->GetMeshData().indices.size(), 0, 0);
    //设置渲染状态
    // m_pImmediateContext->SetRenderMode(TINY3D_FILL_SOLIDE);
    //设置着色器变量
    // m_pShader->SetWorldViewProj(m_worldViewProj);
    // m_pShader->SetWorld(m_world);
    // m_pShader->SetWorldInvTranspose(m_worldInvTranspose);

    // m_pImmediateContext->DrawIndexed(m_indices.size(), 0, 0);
}
PlatForms::Win32Platform *&Model::GetPlatform() {
    return platform_;
}
int Model::GetWidth() {
    return model_draw->GetFrameImage()->GetWidth();
}
int Model::GetHeight() {
    return model_draw->GetFrameImage()->GetHeight();
}
