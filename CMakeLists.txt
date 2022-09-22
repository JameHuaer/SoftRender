cmake_minimum_required(VERSION 3.0.0)
project(SoftRender VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
include(CTest)
enable_testing()
add_executable(SoftRender main.cpp math/Vector.h math/Vector.cpp math/Matrix.h math/Matrix.cpp core/Triangle.h core/Triangle.cpp
core/Texture.h core/Texture.cpp core/Shader.h core/Shader.cpp core/Global.h core/Rasterizer.h core/Rasterizer.cpp core/Camera.h core/Camera.cpp
math/MathUtil.h math/MathUtil.cpp core/FrameImage.h core/FrameImage.cpp core/ObjLoader.h core/ObjLoader.cpp
platforms/Win32.h platforms/Win32.cpp core/ShadowMapping.h core/ShadowMapping.cpp core/RenderUtil.h core/RenderUtil.cpp)#softRender/Render.h softRender/Render.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
