#include "torus.h"
using namespace  std;
Torus::Torus() {}

vector<Torus::Vertex>Torus:: createTorusVertices(float R, float r, int radialSegments, int tubularSegments)
{
  vector<Vertex> vertices;

    for (int i = 0; i <= radialSegments; ++i)
  {
        float theta = 2 * M_PI * i / radialSegments; // 环向角度
        for (int j = 0; j <= tubularSegments; ++j)
        {
            Vertex  vert;
            float phi = 2 * M_PI * j / tubularSegments; // 管状角度

            // 计算位置
            float cx = (R + r * cos(phi)) * cos(theta);
            float cy = (R + r * cos(phi)) * sin(theta);
            float cz = r * sin(phi);
            vert.x = cx, vert.y = cy, vert.z = cz;
            //颜色
            vert.r = 1.0f; vert.g = 1.0f; vert.b = 1.0f;
            // 计算法线
            float nx = cos(phi) * cos(theta);
            float ny = cos(phi) * sin(theta);
            float nz = sin(phi);
            vert.nx = nx, vert.ny = ny, vert.nz = nz;
            // 计算UV坐标
            float u = (float)i / radialSegments;
            float v = (float)j / tubularSegments;
            vert.u = u, vert.v = v;

            vertices.emplace_back(vert);
        }
    }
    return vertices;
}

std::vector<unsigned int> Torus::createTorusIndices(int radialSegments, int tubularSegments) {
    std::vector<unsigned int> indices;

    for (int i = 1; i <= radialSegments; ++i) {
        for (int j = 1; j <= tubularSegments; ++j) {
            int a = (i - 1) * (tubularSegments + 1) + (j - 1);
            int b = i * (tubularSegments + 1) + (j - 1);
            int c = i * (tubularSegments + 1) + j;
            int d = (i - 1) * (tubularSegments + 1) + j;

            // 第一个三角形
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            // 第二个三角形
            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
    return indices;
}

void Torus::initialize()
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
    m_vertices = createTorusVertices(m_R, m_r, m_radialSegments, m_tubularSegments);
    m_indices = createTorusIndices(m_radialSegments, m_tubularSegments);
    computeTangents(m_vertices, m_indices);

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

void Torus::draw(GLuint currentProgram)
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
