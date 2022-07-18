
#ifndef RASTERIZER_SHADER_H
#define RASTERIZER_SHADER_H
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "Texture.h"
#include <vector>
struct FragmentShaderPayload {
    FragmentShaderPayload() {
        texture = nullptr;
    }

    FragmentShaderPayload(const Maths::Vector3f &col, const Maths::Vector3f &nor, const Maths::Vector2f &tc, Texture2D *tex)
        : color(col), normal(nor), tex_coords(tc), texture(tex) {
    }

    Maths::Vector3f view_pos;
    Maths::Vector3f color;
    Maths::Vector3f normal;
    Maths::Vector2f tex_coords;
    Texture2D *texture;
};

struct VertexShaderPayload {
    Maths::Vector3f position;
};
struct Light {
    Maths::Vector3f position;
    Maths::Vector3f intensity;
};

Maths::Vector3f VertexShader(const VertexShaderPayload &payload);

Maths::Vector3f NormalFragmentShader(const FragmentShaderPayload &payload);

static Maths::Vector3f Reflect(const Maths::Vector3f &vec, const Maths::Vector3f &axis);

Maths::Vector3f TextureFragmentShader(const FragmentShaderPayload &payload);
Maths::Vector3f PhongFragmentShader(const FragmentShaderPayload &payload);
Maths::Vector3f DisplacementFragmentShader(const FragmentShaderPayload &payload);
Maths::Vector3f BumpFragmentShader(const FragmentShaderPayload &payload);

#endif // RASTERIZER_SHADER_H
