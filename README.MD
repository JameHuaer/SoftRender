## 启动项目

```cmake
mkdir build ``// 创建build文件夹以保留的工程文件
cd build ``// 进入build文件夹
cmake -G ``"MinGW Makefiles"` `../ ``// 通过提供CMakeLists.txt文件的路径作为参数来运行CMake。
mingw32-make ``// 通过make编译代码
.\SoftRender.exe ``// 运行代码
```

## 操作说明

鼠标左键：旋转

鼠标右键：平移

鼠标中键：缩放

方向键左右：光源绕Y轴旋转

方向键上下：光源绕X轴旋转

空格键：光源停止旋转。