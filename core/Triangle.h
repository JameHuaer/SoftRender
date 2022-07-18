#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include "../math/Matrix.h"
#include "../math/Vector.h"

#include "Texture.h"
#include <array>
class Triangle {

public:
    Maths::Vector4f v[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
    /*Per vertex values*/
    Maths::Vector3f color[3];      // color at each vertex;——颜色
    Maths::Vector2f tex_coords[3]; // texture u,v——纹理
    Maths::Vector3f normal[3];     // normal vector for each vertex——法向量

    Texture2D *tex = nullptr;
    Triangle();
    Maths::Vector4f a() const {
        return v[0];
    }
    Maths::Vector4f b() const {
        return v[1];
    }
    Maths::Vector4f c() const {
        return v[2];
    }

    void setVertex(int ind, Maths::Vector4f ver);      /*set i-th vertex coordinates */
    void setNormal(int ind, Maths::Vector3f n);        /*set i-th vertex normal vector*/
    void setColor(int ind, float r, float g, float b); /*set i-th vertex color*/

    void setVertexes(const std::array<Maths::Vector4f, 3> &vertexes);
    void setNormals(const std::array<Maths::Vector3f, 3> &normals);
    void setColors(const std::array<Maths::Vector3f, 3> &colors);
    void setTexCoord(int ind, Maths::Vector2f uv); /*set i-th vertex texture coordinate*/
    void setTexCoords(const std::array<Maths::Vector2f, 3> &t);
    std::array<Maths::Vector4f, 3> toVector4() const;
};

#endif // RASTERIZER_TRIANGLE_H
