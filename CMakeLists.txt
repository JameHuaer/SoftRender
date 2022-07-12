cmake_minimum_required(VERSION 3.0.0)
project(SoftRender VERSION 0.1.0)

include(CTest)
enable_testing()
add_executable(SoftRender main.cpp softRender/math/Vector.h softRender/math/Vector.cpp softRender/math/MathUtil.h softRender/math/MathUtil.cpp 
softRender/model/Model.h  softRender/model/Model.cpp softRender/model/GeometryGenerator.h  softRender/model/GeometryGenerator.cpp
softRender/model/FrameImage.cpp softRender/model/FrameImage.h softRender/model/Camera.cpp softRender/model/Camera.h
softRender/model/ModelDraw.cpp softRender/model/ModelDraw.h softRender/model/ShaderModel.cpp softRender/model/ShaderModel.h
softRender/platforms/Win32.cpp softRender/platforms/Win32.h)#softRender/Render.h softRender/Render.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
