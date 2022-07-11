#ifndef SHADERBASE_H
#define SHADERBASE_H
#include "../math/Vector.h"
#include "Vertex.h"
namespace ModelSpace {
class ShaderBase {
private:
    /* data */
public:
    ShaderBase() = default;
    virtual ~ShaderBase() = default;
    virtual VertexShadeOut VertexShader(const VertexPrimitive &vin) = 0;
    virtual Maths::Vector4f PixelShader(VertexShadeOut &pin) = 0;
};
}

#endif