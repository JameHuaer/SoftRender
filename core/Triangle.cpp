//
// Created by LEI XU on 4/11/19.
//

#include "Triangle.h"
#include <algorithm>
#include <array>

Triangle::Triangle() {
    v[0] = {0, 0, 0, 1};
    v[1] = {0, 0, 0, 1};
    v[2] = {0, 0, 0, 1};

    color[0] = {0.0, 0.0, 0.0};
    color[1] = {0.0, 0.0, 0.0};
    color[2] = {0.0, 0.0, 0.0};

    tex_coords[0] = {0.0, 0.0};
    tex_coords[1] = {0.0, 0.0};
    tex_coords[2] = {0.0, 0.0};
}

void Triangle::setVertex(int ind, Maths::Vector4f ver) {
    v[ind] = ver;
}
void Triangle::setNormal(int ind, Maths::Vector3f n) {
    normal[ind] = n;
}
void Triangle::setColor(int ind, float r, float g, float b) {
    if ((r < 0.0) || (r > 255.) ||
        (g < 0.0) || (g > 255.) ||
        (b < 0.0) || (b > 255.)) {
        fprintf(stderr, "ERROR! Invalid color values");
        fflush(stderr);
        exit(-1);
    }

    color[ind] = Maths::Vector3f((float)r / 255., (float)g / 255., (float)b / 255.);
    return;
}
void Triangle::setTexCoord(int ind, Maths::Vector2f uv) {
    tex_coords[ind] = uv;
}

std::array<Maths::Vector4f, 3> Triangle::toVector4() const {
    std::array<Maths::Vector4f, 3> res;
    res[0] = Maths::Vector4f{v[0].x, v[0].y, v[0].z, 1.0f};
    res[1] = Maths::Vector4f{v[1].x, v[1].y, v[1].z, 1.0f};
    res[2] = Maths::Vector4f{v[2].x, v[2].y, v[2].z, 1.0f};
    return res;
}

void Triangle::setNormals(const std::array<Maths::Vector3f, 3> &normals) {
    normal[0] = normals[0];
    normal[1] = normals[1];
    normal[2] = normals[2];
}

void Triangle::setColors(const std::array<Maths::Vector3f, 3> &colors) {
    auto first_color = colors[0];
    setColor(0, colors[0][0], colors[0][1], colors[0][2]);
    setColor(1, colors[1][0], colors[1][1], colors[1][2]);
    setColor(2, colors[2][0], colors[2][1], colors[2][2]);
}
void Triangle::setVertexes(const std::array<Maths::Vector4f, 3> &vertexes) {
    v[0] = vertexes[0];
    v[1] = vertexes[1];
    v[2] = vertexes[2];
}
void Triangle::setTexCoords(const std::array<Maths::Vector2f, 3> &t) {
    tex_coords[0] = t[0];
    tex_coords[1] = t[1];
    tex_coords[2] = t[2];
}