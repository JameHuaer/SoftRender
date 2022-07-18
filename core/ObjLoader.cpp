#include "ObjLoader.h"

void ObjLoader::LoadFile(const std::string &file_path) {
    std::ifstream in;
    in.open(file_path, std::ifstream::in);
    if (in.fail())
        return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        //将obj文件中的顶点坐标、纹理坐标、顶点法向量以及f读取
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Maths::Vector3f v;
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            obj_data_.verts.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Maths::Vector3f n;
            for (int i = 0; i < 3; i++)
                iss >> n[i];
            obj_data_.norms.push_back(n.normalize());
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Maths::Vector2f uv;
            for (int i = 0; i < 2; i++)
                iss >> uv[i];
            obj_data_.tex_coord.push_back({uv.u, uv.v}); // modify 法线贴图错误问题所在，
        } else if (!line.compare(0, 2, "f ")) {
            int f, t, n;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                obj_data_.facet_vrt.push_back(--f);
                obj_data_.facet_tex.push_back(--t);
                obj_data_.facet_nrm.push_back(--n);
                cnt++;
            }
            if (3 != cnt) {
                std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
                in.close();
                return;
            }
        }
    }
    in.close();
    // std::cerr << "# v# " << nverts() << " f# " << nfaces() << " vt# " << tex_coord.size() << " vn# " << norms.size() << std::endl;
    // load_texture(filename, "_diffuse.tga", diffusemap);
    // load_texture(filename, "_nm_tangent.tga", normalmap);
    // load_texture(filename, "_spec.tga", specularmap);
}
