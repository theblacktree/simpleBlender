#include "uvsphere.h"
using namespace std;
UVSphere::UVSphere()
{

}

vector<UVSphere::Vertex> UVSphere::createSphereVertices(float radius, int nLongitude, int mLatitude)
{
    vector<Vertex>vertices;
    for (int latNumber = 0; latNumber <= mLatitude; ++latNumber)
    {
        float theta = latNumber * M_PI / mLatitude;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (int longNumber = 0; longNumber <= nLongitude; ++longNumber)
        {
            float phi = longNumber * 2.0f * M_PI / nLongitude;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // 球面位置
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            // 世界空间位置
            float px = radius * x;
            float py = radius * y;
            float pz = radius * z;

            // 法线向量 = 单位球的位置归一化后就是法线
            float nx = x;
            float ny = y;
            float nz = z;

            // UV坐标（经纬映射）
            float u = 1.0f - (float)longNumber / nLongitude;
            float v = (float)latNumber / mLatitude;

            // 构建 Tangent 和 Bitangent（基于 UV 导数方向）
            glm::vec3 pos(x, y, z);
            glm::vec2 uv(u, v);

            // 切线方向：沿经度增加的方向（绕y轴旋转）
            float dphi = 0.001f;
            float next_phi = phi + dphi;
            float sinNextPhi = std::sin(next_phi);
            float cosNextPhi = std::cos(next_phi);
            glm::vec3 tangent(
                cosNextPhi * sinTheta - cosPhi * sinTheta,
                0,
                sinNextPhi * sinTheta - sinPhi * sinTheta
                );
            tangent = glm::normalize(tangent);

            // 副切线方向：叉积
            glm::vec3 bitangent = glm::normalize(glm::cross(pos, tangent));

            Vertex vert;
            vert.x = px; vert.y = py; vert.z = pz;
            vert.r = 1.0f; vert.g = 1.0f; vert.b = 1.0f; //vert.a = 1.0f; // 白色
            vert.nx = nx; vert.ny = ny; vert.nz = nz;
            vert.u = u; vert.v = v;
            vert.tangentX = tangent.x; vert.tangentY = tangent.y; vert.tangentZ = tangent.z;
            vert.bitangentX = bitangent.x; vert.bitangentY = bitangent.y; vert.bitangentZ = bitangent.z;

            vertices.push_back(vert);
        }
    }
    return vertices;
}

std::vector<unsigned int> createSphereIndices(int nLongitude, int mLatitude)
{
    std::vector<unsigned int> indices;
    for (int latNumber = 0; latNumber < mLatitude; ++latNumber)
    {
        for (int longNumber = 0; longNumber < nLongitude; ++longNumber)
        {
            int first = (latNumber * (nLongitude + 1)) + longNumber;
            int second = first + nLongitude + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return indices;
}

void UVSphere::initialize()
{
    initializeOpenGLFunctions();
    // 确认存在有效的 OpenGL 上下文
    glCullFace(GL_BACK);  // 剔除背面
    glFrontFace(GL_CCW);  // 逆时针顺序为正面
    if (!QOpenGLContext::currentContext())
    {
        qWarning() << "No current OpenGL context in Cube::initialize";
        return;
    }
    //顶点
    // GLfloat vertices[] = {
    //     // 位置                    //颜色           //法线方向朝着面外侧    //UV坐标   //切线  //副切线
    // };
    m_vertices = createSphereVertices(m_radius, m_nLongitude, m_mLatitude);
    m_indices = createSphereIndices(m_nLongitude, m_mLatitude);

    // GLuint indices[] = {

    // };
    //calculateTangentBiTangent(vertices, indices, 24, 36);
    // 确保 OpenGL 上下文是当前的
    if (!QOpenGLContext::currentContext()->extraFunctions())
    {
        qCritical("OpenGL functions are not initialized.");
        return;
    }
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW );

    recordError("buffer Error: ");
    //顶点坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    //颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //法线坐标
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //uv纹理坐标
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    //顶点切线
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);
    //顶点副切线
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(float), (void*)(14 * sizeof(float)));
    glEnableVertexAttribArray(5);
    //变换矩阵
    m_model = glm::identity<glm::mat4>();
    // m_view = identity<mat4>();
    // m_projection = identity<mat4>();
    // m_view = translate(m_view, vec3(0.0f, 0.0f, -3.0f));
    // m_projection  = perspective(radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
    glBindVertexArray(0);
}

void UVSphere::draw(GLuint currentProgram)
{
    glBindVertexArray(m_vao);
    recordError("paintGL bind Error: ");
    GLint modelLoc = glGetUniformLocation(currentProgram, "amodel");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_model));

    GLint objectColorLoc = glGetUniformLocation(currentProgram, "objectColor");
    glUniform4fv(objectColorLoc, 1, glm::value_ptr(m_color));

    GLint useObjectColorLoc = glGetUniformLocation(currentProgram, "useObjectColor");
    glUniform1i(useObjectColorLoc, m_isUseObjectColor ? 1 : 0);

    GLint metallicLoc = glGetUniformLocation(currentProgram, "metallic");
    glUniform1f(metallicLoc, m_metallic);
    GLint roughnessLoc = glGetUniformLocation(currentProgram, "roughness");
    glUniform1f(roughnessLoc, m_roughness);
    GLint iorLoc = glGetUniformLocation(currentProgram, "ior");
    glUniform1f(iorLoc, m_ior);

    //texture variety
    GLint useDiffuseMapLoc = glGetUniformLocation(currentProgram, "isUseDiffuseMap");
    glUniform1i(useDiffuseMapLoc, m_diffuseTextureId ? 1 : 0);
    GLint useUseNormalMapLoc = glGetUniformLocation(currentProgram, "isUseNormalMap");
    glUniform1i(useUseNormalMapLoc, m_normalTextureId ? 1 : 0);
    GLint useCubeMapLoc = glGetUniformLocation(currentProgram, "isUseCubeMap");
    glUniform1i(useCubeMapLoc, m_cubeTextureId ? 1 : 0);
    //follow is bing texture

    // bind Texture
    // Activate and bind the diffuse texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_diffuseTextureId);

    // Activate and bind the normal texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalTextureId);

    // Activate and bind the specular texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTextureId);



    // Assuming you have a shader program that expects these textures in specific texture units
    glUniform1i(glGetUniformLocation(currentProgram, "diffuseMap"), 0); // GL_TEXTURE0
    glUniform1i(glGetUniformLocation(currentProgram, "normalMap"), 1);   // GL_TEXTURE1
    glUniform1i(glGetUniformLocation(currentProgram, "cubeMap"), 2); // GL_TEXTURE2

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
    recordError("cube Draw Error: ");

    glBindVertexArray(0);
}

