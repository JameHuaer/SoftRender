#include "GeometryGenerator.h"
void ModelSpace::GeometryGenerator::CreateBox(float width, float height, float depth, MeshData &mesh) {
    mesh.vertices.clear();
    mesh.indices.clear();

    //一共24个顶点(每面4个)
    mesh.vertices.resize(24);
    //一共36个索引(每面6个)
    mesh.indices.resize(36);

    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfD = depth * 0.5f;

    //眼睛面向z轴正方向
    //构建顶点
    //前面
    mesh.vertices[0].position = Maths::Vector4f(-halfW, -halfH, -halfD, 1.f);
    mesh.vertices[0].normal = Maths::Vector4f(0.f, 0.f, -1.f);
    mesh.vertices[0].color = Maths::Vector4f(1.f, 0.f, 0.f, 1.f);
    mesh.vertices[0].texcoord = Maths::Vector2f(0.f, 1.f);
    mesh.vertices[1].position = Maths::Vector4f(-halfW, halfH, -halfD, 1.f);
    mesh.vertices[1].normal = Maths::Vector4f(0.f, 0.f, -1.f);
    mesh.vertices[1].color = Maths::Vector4f(0.f, 0.f, 0.f, 1.f);
    mesh.vertices[1].texcoord = Maths::Vector2f(0.f, 0.f);
    mesh.vertices[2].position = Maths::Vector4f(halfW, halfH, -halfD, 1.f);
    mesh.vertices[2].normal = Maths::Vector4f(0.f, 0.f, -1.f);
    mesh.vertices[2].color = Maths::Vector4f(1.f, 0.f, 0.f, 1.f);
    mesh.vertices[2].texcoord = Maths::Vector2f(1.f, 0.f);
    mesh.vertices[3].position = Maths::Vector4f(halfW, -halfH, -halfD, 1.f);
    mesh.vertices[3].normal = Maths::Vector4f(0.f, 0.f, -1.f);
    mesh.vertices[3].color = Maths::Vector4f(0.f, 1.f, 0.f, 1.f);
    mesh.vertices[3].texcoord = Maths::Vector2f(1.f, 1.f);
    //左侧面
    mesh.vertices[4].position = Maths::Vector4f(-halfW, -halfH, halfD, 1.f);
    mesh.vertices[4].normal = Maths::Vector4f(-1.f, 0.f, 0.f);
    mesh.vertices[4].color = Maths::Vector4f(0.f, 0.f, 1.f, 1.f);
    mesh.vertices[4].texcoord = Maths::Vector2f(0.f, 1.f);
    mesh.vertices[5].position = Maths::Vector4f(-halfW, halfH, halfD, 1.f);
    mesh.vertices[5].normal = Maths::Vector4f(-1.f, 0.f, 0.f);
    mesh.vertices[5].color = Maths::Vector4f(1.f, 1.f, 0.f, 1.f);
    mesh.vertices[5].texcoord = Maths::Vector2f(0.f, 0.f);
    mesh.vertices[6].position = Maths::Vector4f(-halfW, halfH, -halfD, 1.f);
    mesh.vertices[6].normal = Maths::Vector4f(-1.f, 0.f, 0.f);
    mesh.vertices[6].color = Maths::Vector4f(0.f, 0.f, 0.f, 1.f);
    mesh.vertices[6].texcoord = Maths::Vector2f(1.f, 0.f);
    mesh.vertices[7].position = Maths::Vector4f(-halfW, -halfH, -halfD, 1.f);
    mesh.vertices[7].normal = Maths::Vector4f(-1.f, 0.f, 0.f);
    mesh.vertices[7].color = Maths::Vector4f(1.f, 1.f, 1.f, 1.f);
    mesh.vertices[7].texcoord = Maths::Vector2f(1.f, 1.f);
    //背面
    mesh.vertices[8].position = Maths::Vector4f(halfW, -halfH, halfD, 1.f);
    mesh.vertices[8].normal = Maths::Vector4f(0.f, 0.f, 1.f);
    mesh.vertices[8].color = Maths::Vector4f(1.f, 0.f, 1.f, 1.f);
    mesh.vertices[8].texcoord = Maths::Vector2f(0.f, 1.f);
    mesh.vertices[9].position = Maths::Vector4f(halfW, halfH, halfD, 1.f);
    mesh.vertices[9].normal = Maths::Vector4f(0.f, 0.f, 1.f);
    mesh.vertices[9].color = Maths::Vector4f(0.f, 1.f, 1.f, 1.f);
    mesh.vertices[9].texcoord = Maths::Vector2f(0.f, 0.f);
    mesh.vertices[10].position = Maths::Vector4f(-halfW, halfH, halfD, 1.f);
    mesh.vertices[10].normal = Maths::Vector4f(0.f, 0.f, 1.f);
    mesh.vertices[10].color = Maths::Vector4f(1.f, 1.f, 0.f, 1.f);
    mesh.vertices[10].texcoord = Maths::Vector2f(1.f, 0.f);
    mesh.vertices[11].position = Maths::Vector4f(-halfW, -halfH, halfD, 1.f);
    mesh.vertices[11].normal = Maths::Vector4f(0.f, 0.f, 1.f);
    mesh.vertices[11].color = Maths::Vector4f(0.f, 0.f, 1.f, 1.f);
    mesh.vertices[11].texcoord = Maths::Vector2f(1.f, 1.f);
    //右侧面
    mesh.vertices[12].position = Maths::Vector4f(halfW, -halfH, -halfD, 1.f);
    mesh.vertices[12].normal = Maths::Vector4f(1.f, 0.f, 0.f);
    mesh.vertices[12].color = Maths::Vector4f(0.f, 1.f, 0.f, 1.f);
    mesh.vertices[12].texcoord = Maths::Vector2f(0.f, 1.f);
    mesh.vertices[13].position = Maths::Vector4f(halfW, halfH, -halfD, 1.f);
    mesh.vertices[13].normal = Maths::Vector4f(1.f, 0.f, 0.f);
    mesh.vertices[13].color = Maths::Vector4f(1.f, 0.f, 0.f, 1.f);
    mesh.vertices[13].texcoord = Maths::Vector2f(0.f, 0.f);
    mesh.vertices[14].position = Maths::Vector4f(halfW, halfH, halfD, 1.f);
    mesh.vertices[14].normal = Maths::Vector4f(1.f, 0.f, 0.f);
    mesh.vertices[14].color = Maths::Vector4f(0.f, 1.f, 1.f, 1.f);
    mesh.vertices[14].texcoord = Maths::Vector2f(1.f, 0.f);
    mesh.vertices[15].position = Maths::Vector4f(halfW, -halfH, halfD, 1.f);
    mesh.vertices[15].normal = Maths::Vector4f(1.f, 0.f, 0.f);
    mesh.vertices[15].color = Maths::Vector4f(1.f, 0.f, 1.f, 1.f);
    mesh.vertices[15].texcoord = Maths::Vector2f(1.f, 1.f);
    //上面
    mesh.vertices[16].position = Maths::Vector4f(-halfW, halfH, -halfD, 1.f);
    mesh.vertices[16].normal = Maths::Vector4f(0.f, 1.f, 0.f);
    mesh.vertices[16].color = Maths::Vector4f(0.f, 0.f, 0.f, 1.f);
    mesh.vertices[16].texcoord = Maths::Vector2f(0.f, 1.f);
    mesh.vertices[17].position = Maths::Vector4f(-halfW, halfH, halfD, 1.f);
    mesh.vertices[17].normal = Maths::Vector4f(0.f, 1.f, 0.f);
    mesh.vertices[17].color = Maths::Vector4f(1.f, 1.f, 0.f, 1.f);
    mesh.vertices[17].texcoord = Maths::Vector2f(0.f, 0.f);
    mesh.vertices[18].position = Maths::Vector4f(halfW, halfH, halfD, 1.f);
    mesh.vertices[18].normal = Maths::Vector4f(0.f, 1.f, 0.f);
    mesh.vertices[18].color = Maths::Vector4f(0.f, 1.f, 1.f, 1.f);
    mesh.vertices[18].texcoord = Maths::Vector2f(1.f, 0.f);
    mesh.vertices[19].position = Maths::Vector4f(halfW, halfH, -halfD, 1.f);
    mesh.vertices[19].normal = Maths::Vector4f(0.f, 1.f, 0.f);
    mesh.vertices[19].color = Maths::Vector4f(1.f, 0.f, 0.f, 1.f);
    mesh.vertices[19].texcoord = Maths::Vector2f(1.f, 1.f);
    //底面
    mesh.vertices[20].position = Maths::Vector4f(-halfW, -halfH, halfD, 1.f);
    mesh.vertices[20].normal = Maths::Vector4f(0.f, -1.f, 0.f);
    mesh.vertices[20].color = Maths::Vector4f(0.f, 0.f, 1.f, 1.f);
    mesh.vertices[20].texcoord = Maths::Vector2f(0.f, 1.f);
    mesh.vertices[21].position = Maths::Vector4f(-halfW, -halfH, -halfD, 1.f);
    mesh.vertices[21].normal = Maths::Vector4f(0.f, -1.f, 0.f);
    mesh.vertices[21].color = Maths::Vector4f(1.f, 1.f, 1.f, 1.f);
    mesh.vertices[21].texcoord = Maths::Vector2f(0.f, 0.f);
    mesh.vertices[22].position = Maths::Vector4f(halfW, -halfH, -halfD, 1.f);
    mesh.vertices[22].normal = Maths::Vector4f(0.f, -1.f, 0.f);
    mesh.vertices[22].color = Maths::Vector4f(0.f, 1.f, 0.f, 1.f);
    mesh.vertices[22].texcoord = Maths::Vector2f(1.f, 0.f);
    mesh.vertices[23].position = Maths::Vector4f(halfW, -halfH, halfD, 1.f);
    mesh.vertices[23].normal = Maths::Vector4f(0.f, -1.f, 0.f);
    mesh.vertices[23].color = Maths::Vector4f(1.f, 0.f, 1.f, 1.f);
    mesh.vertices[23].texcoord = Maths::Vector2f(1.f, 1.f);

    //构建索引
    for (int i = 0, j = 0; i < 21; i += 6, j += 4) {
        mesh.indices[i] = j;
        mesh.indices[i + 1] = j + 1;
        mesh.indices[i + 2] = j + 2;
        mesh.indices[i + 3] = j;
        mesh.indices[i + 4] = j + 2;
        mesh.indices[i + 5] = j + 3;
    }
}
