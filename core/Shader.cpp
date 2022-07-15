#include "Shader.h"

Maths::Vector3f vertex_shader(const vertex_shader_payload &payload) {
    return payload.position;
}

Maths::Vector3f normal_fragment_shader(const fragment_shader_payload &payload) {
    Maths::Vector3f return_color = (payload.normal.normalize() + Maths::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    // Maths::Vector3f result;
    // result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;

    return return_color;
}

static Maths::Vector3f reflect(const Maths::Vector3f &vec, const Maths::Vector3f &axis) {
    auto costheta = vec.Dot(axis);
    return (2 * costheta * axis - vec).normalize();
}
Maths::Vector3f texture_fragment_shader(const fragment_shader_payload &payload) {
    Maths::Vector3f return_color = {0, 0, 0};
    if (payload.texture) {
        return_color = payload.texture->getColorBilinear(payload.tex_coords.u, payload.tex_coords.v); //使用双线性插值
        // return_color = payload.texture->getColor(payload.tex_coords.x(), payload.tex_coords.y());
    }
    Maths::Vector3f texture_color;
    texture_color = {return_color.x, return_color.y, return_color.z};

    Maths::Vector3f ka = Maths::Vector3f(0.005, 0.005, 0.005);
    Maths::Vector3f kd = texture_color; 
    Maths::Vector3f ks = Maths::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Maths::Vector3f amb_light_intensity{10, 10, 10};
    Maths::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Maths::Vector3f color = texture_color;
    Maths::Vector3f point = payload.view_pos;
    Maths::Vector3f normal = payload.normal;

    Maths::Vector3f result_color = {0, 0, 0};

    Maths::Vector3f l, v, h, ambient, diffuse, specular;
    for (auto &light : lights) {
        l = (light.position - point).normalize();
        v = (eye_pos - point).normalize();
        h = (l + v).normalize();
        ambient = ka.CWiseProduct(amb_light_intensity);
        diffuse = kd.CWiseProduct(light.intensity / (light.position - point).Dot(light.position - point)) * std::fmax(0, normal.Dot(l));
        specular = ks.CWiseProduct(light.intensity / (light.position - point).Dot(light.position - point)) * std::pow(std::fmax(0, normal.Dot(h)), p);
        // (std::cout << specular << std::endl);
        result_color = result_color + (ambient + diffuse + specular);
    }
    result_color.x = result_color.x > 1.0f ? 1.0f : result_color.x;
    result_color.y = result_color.y > 1.0f ? 1.0f : result_color.y;
    result_color.z = result_color.z > 1.0f ? 1.0f : result_color.z;
    return result_color; 
}

Maths::Vector3f phong_fragment_shader(const fragment_shader_payload &payload) {
    Maths::Vector3f result_color = {0, 0, 0};

    Maths::Vector3f ka = Maths::Vector3f(0.005, 0.005, 0.005);    //环境光
    Maths::Vector3f kd = payload.color;                           //漫反射
    Maths::Vector3f ks = Maths::Vector3f(0.7937, 0.7937, 0.7937); //高光系数

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Maths::Vector3f amb_light_intensity{10, 10, 10}; //环境光强
    Maths::Vector3f eye_pos{0, 0, 10};               //眼睛观测位置

    float p = 150;

    Maths::Vector3f color = payload.color;    //纹理颜色
    Maths::Vector3f point = payload.view_pos; //着色点坐标
    Maths::Vector3f normal = payload.normal;  //着色点法向量

    Maths::Vector3f l, v, h, ambient, diffuse, specular;
    for (auto &light : lights) {
        l = (light.position - point).normalize();
        v = (eye_pos - point).normalize();
        h = (l + v).normalize();
        ambient = ka.CWiseProduct(amb_light_intensity);
        diffuse = kd.CWiseProduct(light.intensity / (light.position - point).Dot(light.position - point)) * std::fmax(0, normal.Dot(l));
        specular = ks.CWiseProduct(light.intensity / (light.position - point).Dot(light.position - point)) * std::pow(std::fmax(0, normal.Dot(h)), p);
        result_color = result_color + (ambient + diffuse + specular);

    }
    result_color.x = result_color.x > 1.0f ? 1.0f : result_color.x;
    result_color.y = result_color.y > 1.0f ? 1.0f : result_color.y;
    result_color.z = result_color.z > 1.0f ? 1.0f : result_color.z;

    return result_color; 
}

Maths::Vector3f displacement_fragment_shader(const fragment_shader_payload &payload) {

    Maths::Vector3f ka = Maths::Vector3f(0.005, 0.005, 0.005);
    Maths::Vector3f kd = payload.color;
    Maths::Vector3f ks = Maths::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Maths::Vector3f amb_light_intensity{10, 10, 10};
    Maths::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Maths::Vector3f color = payload.color;
    Maths::Vector3f point = payload.view_pos;
    Maths::Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;

    Maths::Vector3f n = normal;
    float x = normal.x, y = normal.y, z = normal.z;
    Maths::Vector3f t = {x * y / (float)sqrt(x * x + z * z), (float)sqrt(x * x + z * z), z * y / (float)sqrt(x * x + z * z)};
    Maths::Vector3f b = n.Cross(t);
    Maths::Matrix3f TBN;
    TBN = {{{t.x, t.y, t.z},
            {b.x, b.y, b.z},
            {n.x, n.y, n.z}}};

    float u, v, w, h;
    u = payload.tex_coords.u;
    v = payload.tex_coords.v;
    w = payload.texture->width_;
    h = payload.texture->height_;
    float du = kh * kn * (payload.texture->getColorBilinear(u + 1.0 / w, v).norm() - payload.texture->getColorBilinear(u, v).norm());
    float dv = kh * kn * (payload.texture->getColorBilinear(u, v + 1.0 / h).norm() - payload.texture->getColorBilinear(u, v).norm());
    Maths::Vector3f ln(-du, dv, 1.0);
    point = point + kn * n * payload.texture->getColorBilinear(u, v).norm();
    normal = (TBN * ln).normalize();

    Maths::Vector3f result_color = {0, 0, 0};

    Maths::Vector3f l, v_eye, h_vector, ambient, diffuse, specular;
    for (auto &light : lights) {
        l = (light.position - point).normalize();
        v_eye = (eye_pos - point).normalize();
        h_vector = (l + v_eye).normalize();
        ambient = ka.CWiseProduct(amb_light_intensity);
        diffuse = kd.CWiseProduct(light.intensity / (light.position - point).Dot(light.position - point)) * std::fmax(0, normal.Dot(l));
        specular = ks.CWiseProduct(light.intensity / (light.position - point).Dot(light.position - point)) * std::pow(std::fmax(0, normal.Dot(h_vector)), p);
        result_color = result_color + (ambient + diffuse + specular);
    }
    result_color.x = result_color.x > 1.0f ? 1.0f : result_color.x;
    result_color.y = result_color.y > 1.0f ? 1.0f : result_color.y;
    result_color.z = result_color.z > 1.0f ? 1.0f : result_color.z;

    return result_color;
}

Maths::Vector3f bump_fragment_shader(const fragment_shader_payload &payload) {

    Maths::Vector3f ka = Maths::Vector3f(0.005, 0.005, 0.005);
    Maths::Vector3f kd = payload.color;
    Maths::Vector3f ks = Maths::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Maths::Vector3f amb_light_intensity{10, 10, 10};
    Maths::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Maths::Vector3f color = payload.color;
    Maths::Vector3f point = payload.view_pos;
    Maths::Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;

    float x = normal.x, y = normal.y, z = normal.z;
    Maths::Vector3f t = {x * y / (float)sqrt(x * x + z * z), (float)sqrt(x * x + z * z), z * y / (float)sqrt(x * x + z * z)};
    Maths::Vector3f b = normal.Cross(t);
    Maths::Matrix3f TBN;
    TBN = {{{t.x, t.y, t.z},
            {b.x, b.y, b.z},
            {normal.x, normal.y, normal.z}}};
    float u, v, w, h;
    u = payload.tex_coords.u;
    v = payload.tex_coords.v;
    w = payload.texture->width_;
    h = payload.texture->height_;
    float du = kh * kn * (payload.texture->getColorBilinear(u + 1.0 / w, v).norm() - payload.texture->getColorBilinear(u, v).norm());
    float dv = kh * kn * (payload.texture->getColorBilinear(u, v + 1.0 / h).norm() - payload.texture->getColorBilinear(u, v).norm());
    Maths::Vector3f ln = {-du, dv, 1.0};
    normal = (TBN * ln).normalize();

    Maths::Vector3f result_color = {0, 0, 0};
    result_color = normal;

    result_color.x = result_color.x > 1.0f ? 1.0f : result_color.x;
    result_color.y = result_color.y > 1.0f ? 1.0f : result_color.y;
    result_color.z = result_color.z > 1.0f ? 1.0f : result_color.z;

    return result_color; 
}
