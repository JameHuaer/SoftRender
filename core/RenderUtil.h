#ifndef RENDERUTIL_H
#define RENDERUTIL_H

#include "../math/MathUtil.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "Triangle.h"

bool IsBackFaceCulling(const std::array<Maths::Vector3f, 3> &vecs);

Maths::Matrix4f LookAt(Maths::Vector3f eye, Maths::Vector3f center, Maths::Vector3f up);

#endif