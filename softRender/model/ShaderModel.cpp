#include "ShaderModel.h"
using namespace ModelSpace;
VertexShadeOut ShaderModel::VertexShader(const VertexPrimitive &vin) {
    VertexShadeOut out;
    out.view_projection = vin.position * camera->GetWVPMatrix();
    out.world_view = vin.position * camera->GetWorldMatrix();
    out.normal = out.normal * camera->GetWITMatrix();
    out.color = vin.color;
    out.texcoord = vin.texcoord;
    return out;

    // out.view_projection = vin.position*
}
Maths::Vector4f ShaderModel::PixelShader(VertexShadeOut &pin) {
    pin.normal.normalize();
    return Maths::Vector4f();
}
ShaderModel::ShaderModel(Camera *car)
    : camera(car) {
}

ShaderModel::~ShaderModel() {
}