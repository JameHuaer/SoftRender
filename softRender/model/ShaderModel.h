#ifndef SHADERMODEL_H
#define SHADERMODEL_H
#include "../math/Vector.h"
#include "Camera.h"
#include "ShaderBase.h"
#include "Vertex.h"
namespace ModelSpace {
class ShaderModel : public ShaderBase {
private:
    Camera *camera;

public:
    ShaderModel() = default;
    ShaderModel(Camera *car);
    ~ShaderModel();
    virtual VertexShadeOut VertexShader(const VertexPrimitive &vin);
    virtual Maths::Vector4f PixelShader(VertexShadeOut &pin);
};

}

#endif