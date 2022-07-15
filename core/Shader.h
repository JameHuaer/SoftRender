
#ifndef RASTERIZER_SHADER_H
#define RASTERIZER_SHADER_H
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "Texture.h"
#include <vector>
struct fragment_shader_payload {
    fragment_shader_payload() {
        texture = nullptr;
    }

    fragment_shader_payload(const Maths::Vector3f &col, const Maths::Vector3f &nor, const Maths::Vector2f &tc, Texture2D *tex)
        : color(col), normal(nor), tex_coords(tc), texture(tex) {
    }

    Maths::Vector3f view_pos;
    Maths::Vector3f color;
    Maths::Vector3f normal;
    Maths::Vector2f tex_coords;
    Texture2D *texture;
};

struct vertex_shader_payload {
    Maths::Vector3f position;
};
struct light {
    Maths::Vector3f position;
    Maths::Vector3f intensity;
};

Maths::Vector3f vertex_shader(const vertex_shader_payload &payload);

Maths::Vector3f normal_fragment_shader(const fragment_shader_payload &payload);

static Maths::Vector3f reflect(const Maths::Vector3f &vec, const Maths::Vector3f &axis);

Maths::Vector3f texture_fragment_shader(const fragment_shader_payload &payload);
Maths::Vector3f phong_fragment_shader(const fragment_shader_payload &payload);
Maths::Vector3f displacement_fragment_shader(const fragment_shader_payload &payload);
Maths::Vector3f bump_fragment_shader(const fragment_shader_payload &payload);

#endif // RASTERIZER_SHADER_H
