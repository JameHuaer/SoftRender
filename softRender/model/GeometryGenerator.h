#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H
#include "Vertex.h"
#include <string>
#include <vector>
namespace ModelSpace {
    //基本网格结构，顶点和索引
    struct MeshData {
        std::vector<VertexPrimitive> vertices;
        std::vector<uint32_t> indices;
    };
    //几何体生成单例
    class GeometryGenerator {
    public:
        static GeometryGenerator *GetInstace() {
            static GeometryGenerator instace;
            return &instace;
        }
        void CreateBox(float width, float height, float depth, MeshData &mesh); 
        void LoadOBJModel(const std::string &obj_file_Name, MeshData &mesh);    // todod implement

    private:
        GeometryGenerator() = default;
    };

}

#endif