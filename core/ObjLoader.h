#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "../math/Vector.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//初始模型数据
struct ObjData {
    std::vector<Maths::Vector3f> verts{};     //模型顶点坐标
    std::vector<Maths::Vector2f> tex_coord{}; //模型纹理坐标
    std::vector<Maths::Vector3f> norms{};     //模型法向量坐标
    std::vector<int> facet_vrt{};             //顶点索引
    std::vector<int> facet_tex{};             //纹理索引
    std::vector<int> facet_nrm{};             //法向量索引
};
class ObjLoader {
public:
    ObjData obj_data_;

    ObjLoader() = default;
    ~ObjLoader() = default;
    void LoadFile(const std::string &file_path);
};

#endif