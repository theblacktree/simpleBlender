#include "object.h"
#include <qcolor.h>
#include <qmatrix4x4.h>
using namespace std;
using namespace glm;

Object::Object():m_position(0.0f, 0.0f,0.0f), m_rotation(0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f,1.0f), \
    m_metallic (0.0f), m_roughness (0.04), m_ior (1.450)
{
    // m_isDragging = false;

    // m_rotateX = 0;
    // m_rotateY = 0;
    // m_rotateZ = 0;
}

void Object::setModelMatrix(glm::vec3 &translationXYZ, glm::vec3 &rotationXYZ, glm::vec3 &scaleXYZ)
{
    //设置平移旋转缩放
    if (scaleXYZ.x == 0.0f || scaleXYZ.y == 0.0f || scaleXYZ.z == 0.0f)
    {
        std::cerr << "Error: Scale values cannot be zero!" << std::endl;
        return;
    }
    // 每次调用都会重置 m_model 为单位矩阵
    m_model = mat4(1.0f);
    // 应用缩放
    m_model = glm::scale(m_model, scaleXYZ);
    // 应用旋转。注意：旋转的顺序很重要！这里我们按ZYX顺序旋转
    m_model = rotate(m_model, radians(rotationXYZ.z), vec3(0.0f, 0.0f, 1.0f)); // 绕Z轴旋转
    m_model = rotate(m_model, radians(rotationXYZ.y), vec3(0.0f, 1.0f, 0.0f)); // 绕Y轴旋转
    m_model = rotate(m_model, radians(rotationXYZ.x), vec3(1.0f, 0.0f, 0.0f)); // 绕X轴旋转
    // 应用平移
    m_model = translate(m_model, translationXYZ);
    m_position = translationXYZ;
    m_rotation = rotationXYZ;
    m_scale = scaleXYZ;
}



/*void Object::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    m_projection = identity<mat4>();
    m_projection = perspective(radians(45.0f),static_cast<float>(w)/h, 0.1f, 100.0f);
}
*/
void Object::setObjectColor(QColor& color)
{
    m_color = toolFunc::GetInstance().qColor4ToVec4(color);
    m_isUseObjectColor = true;
}

void Object::setMaterialValue(float metallic, float roughness, float ior)
{
    m_metallic = metallic;
    m_roughness = roughness;
    m_ior = ior;
}

std::vector<float> Object::getMaterialValue()
{
    vector<float> texture;
    texture.emplace_back(m_metallic);
    texture.emplace_back(m_roughness);
    texture.emplace_back(m_ior);
    return texture;
}

 void Object::recordError(const char* s)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qCritical() << s << error;
    }
}

void Object::calculateTangentBiTangent(GLfloat vertices[], GLuint indices[], size_t verticesRowCount, size_t indicesSize)
{
    for (size_t i = 0; i < indicesSize; i += 3)
    {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i + 1];
        unsigned int index2 = indices[i + 2];

        GLfloat pos0x = vertices[index0*17];
        GLfloat pos0y = vertices[index0*17 +1];
        GLfloat pos0z = vertices[index0*17 +2];
        GLfloat pos1x = vertices[index1*17];
        GLfloat pos1y = vertices[index1*17 +1];
        GLfloat pos1z = vertices[index1*17 +2];
        GLfloat pos2x = vertices[index2*17];
        GLfloat pos2y = vertices[index2*17 +1];
        GLfloat pos2z = vertices[index2*17 +2];

        GLfloat uv0_u = vertices[index0 * 17 + 9];
        GLfloat uv0_v = vertices[index0 * 17 + 10];
        GLfloat uv1_u = vertices[index1 * 17 + 9];
        GLfloat uv1_v = vertices[index1 * 17 + 10];
        GLfloat uv2_u = vertices[index2 * 17 + 9];
        GLfloat uv2_v = vertices[index2 * 17 + 10];

        GLfloat ege1x = pos1x - pos0x;
        GLfloat ege2x = pos2x - pos0x;
        GLfloat ege1y = pos1y - pos0y;
        GLfloat ege2y = pos2y - pos0y;
        GLfloat ege1z = pos1z - pos0z;
        GLfloat ege2z = pos2z - pos0z;

        GLfloat deltaUV1_U = uv1_u - uv0_u;
        GLfloat deltaUV1_V = uv1_v - uv0_v;
        GLfloat deltaUV2_U = uv2_u - uv0_u;
        GLfloat deltaUV2_V = uv2_v - uv0_v;

        float f = 1.0f / (deltaUV1_U * deltaUV2_V - deltaUV2_U * deltaUV1_V);

        GLfloat tangentx = f * deltaUV2_V * ege1x - deltaUV1_V * ege2x;
        GLfloat tangenty = f * deltaUV2_V * ege1y - deltaUV1_V * ege2y;
        GLfloat tangentz = f * deltaUV2_V * ege1z - deltaUV1_V * ege2z;

        GLfloat bitangentx = f * (-deltaUV2_U * ege1x + deltaUV1_U * ege2x);
        GLfloat bitangenty = f * (-deltaUV2_U * ege1y + deltaUV1_U * ege2y);
        GLfloat bitangentz = f * (-deltaUV2_U * ege1z + deltaUV1_U * ege2z);

        vertices[index0 * 17+11] += tangentx;
        vertices[index0 * 17+12] += tangenty;
        vertices[index0 * 17+13] += tangentz;
        vertices[index1 * 17+11] += tangentx;
        vertices[index1 * 17+12] += tangenty;
        vertices[index1 * 17+13] += tangentz;
        vertices[index2 * 17+11] += tangentx;
        vertices[index2 * 17+12] += tangenty;
        vertices[index2 * 17+13] += tangentz;

        vertices[index0 * 17+14] += bitangentx;
        vertices[index0 * 17+15] += bitangenty;
        vertices[index0 * 17+16] += bitangentz;
        vertices[index1 * 17+14] += bitangentx;
        vertices[index1 * 17+15] += bitangenty;
        vertices[index1 * 17+16] += bitangentz;
        vertices[index2 * 17+14] += bitangentx;
        vertices[index2 * 17+15] += bitangenty;
        vertices[index2 * 17+16] += bitangentz;
    }
    // Normalize tangents and ensure they are orthogonal to normals
    for (size_t index = 0; index <verticesRowCount; ++index)
    {//index是行数
        glm::vec3 N;
        N.x = vertices[index * 17 +6]; // 法线
        N.y = vertices[index * 17 +7]; // 法线
        N.z = vertices[index * 17 +8]; // 法线

        glm::vec3 T;
        T.x = vertices[index * 17 + 11]; // 切线
        T.y = vertices[index * 17 + 12]; // 切线
        T.z = vertices[index * 17 + 13]; // 切线

        glm::vec3 accumulatedB;
        accumulatedB.x = vertices[index * 17 + 14];
        accumulatedB.y = vertices[index * 17 + 15];
        accumulatedB.z = vertices[index * 17 + 16];

        // 使切线与法线正交
        T = glm::normalize(T - N * glm::dot(N, T));

        // 计算副切线
        glm::vec3 B = glm::cross(N, T);

        // 如果需要，可以对副切线也归一化
        B = glm::normalize(B);
        accumulatedB = glm::normalize(accumulatedB);

        // Calculate handedness
        if (glm::dot(accumulatedB, B) < 0.0f) {
            T *= -1.0f;
        }
        vertices[index * 17 + 11] = T.x; // 切线
        vertices[index * 17 + 12] = T.y; // 切线
        vertices[index * 17 + 13] = T.z; // 切线
        vertices[index * 17 + 14] = B.x;
        vertices[index * 17 + 15] = B.y;
        vertices[index * 17 + 16] = B.z;
    }
}

void Object::computeTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3 edge1(vertices[i1].x - vertices[i0].x, vertices[i1].y - vertices[i0].y, vertices[i1].z - vertices[i0].z);
        glm::vec3 edge2(vertices[i2].x - vertices[i0].x, vertices[i2].y - vertices[i0].y, vertices[i2].z - vertices[i0].z);

        glm::vec2 deltaUV1(vertices[i1].u - vertices[i0].u, vertices[i1].v - vertices[i0].v);
        glm::vec2 deltaUV2(vertices[i2].u - vertices[i0].u, vertices[i2].v - vertices[i0].v);

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent, bitangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        vertices[i0].tangentX += tangent.x; vertices[i0].tangentY += tangent.y; vertices[i0].tangentZ += tangent.z;
        vertices[i0].bitangentX += bitangent.x; vertices[i0].bitangentY += bitangent.y; vertices[i0].bitangentZ += bitangent.z;

        vertices[i1].tangentX += tangent.x; vertices[i1].tangentY += tangent.y; vertices[i1].tangentZ += tangent.z;
        vertices[i1].bitangentX += bitangent.x; vertices[i1].bitangentY += bitangent.y; vertices[i1].bitangentZ += bitangent.z;

        vertices[i2].tangentX += tangent.x; vertices[i2].tangentY += tangent.y; vertices[i2].tangentZ += tangent.z;
        vertices[i2].bitangentX += bitangent.x; vertices[i2].bitangentY += bitangent.y; vertices[i2].bitangentZ += bitangent.z;
    }

    // Normalize tangents and bitangents
    for (auto& vertex : vertices) {
        glm::vec3 tangent(vertex.tangentX, vertex.tangentY, vertex.tangentZ);
        glm::vec3 bitangent(vertex.bitangentX, vertex.bitangentY, vertex.bitangentZ);
        glm::vec3 normal(vertex.nx, vertex.ny, vertex.nz);

        tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));
        bitangent = glm::normalize(bitangent - normal * glm::dot(normal, bitangent) - tangent * glm::dot(tangent, bitangent));

        vertex.tangentX = tangent.x; vertex.tangentY = tangent.y; vertex.tangentZ = tangent.z;
        vertex.bitangentX = bitangent.x; vertex.bitangentY = bitangent.y; vertex.bitangentZ = bitangent.z;
    }
}

void Object::removeUVTexture(TextureType textureType)
{
    if (textureType == TextureType::DIFFUSE_MAP)
    {
        TextureManager::GetInstance().removeOneTexture(m_diffuseTextureId);
        m_diffuseTextureId = 0;
    }
    else if (textureType == TextureType::NORMAL_MAP)
    {
        TextureManager::GetInstance().removeOneTexture(m_normalTextureId);
        m_normalTextureId = 0;
    }
    else
    {
        //立方体贴图先保留
    }
}


