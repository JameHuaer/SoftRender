cmake_minimum_required(VERSION 3.0.0)
project(SoftRender VERSION 0.1.0)

include(CTest)
enable_testing()
add_executable(SoftRender main.cpp softRender/math/Vector.h softRender/math/Vector.cpp softRender/math/MathUtil.h softRender/math/MathUtil.cpp 
softRender/Camera.h softRender/Camera.cpp )#softRender/Render.h softRender/Render.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
