#include "cube.h"

using namespace std;
using namespace glm;
cube::cube()
{

}
cube::~cube()
{

}


void cube::initialize()
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
    GLfloat vertices[] = {
        // 位置                    //颜色           //法线方向朝着面外侧    //UV坐标   //切线  //副切线
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,     0, 1,  0, 0, 0,  0, 0, 0,   // 0
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,     1, 1,  0, 0, 0,  0, 0, 0,  // 1
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,     1, 0,  0, 0, 0,  0, 0, 0,  // 2
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,     0, 0,  0, 0, 0,  0, 0, 0,  // 3

        -0.5f,  0.5f, -0.5f, 0.5f, 0.3f, 0.7f,   0.0f,  0.0f, -1.0f,     1, 1,  0, 0, 0,  0, 0, 0,  // 4
        0.5f,  0.5f, -0.5f,  0.5f, 0.3f, 0.7f,   0.0f,  0.0f, -1.0f,     0, 1,  0, 0, 0,  0, 0, 0,// 5
        0.5f, -0.5f, -0.5f,  0.5f, 0.3f, 0.7f,   0.0f,  0.0f, -1.0f,     0, 0,  0, 0, 0,  0, 0, 0,  // 6
        -0.5f, -0.5f, -0.5f, 0.5f, 0.3f, 0.7f,   0.0f,  0.0f, -1.0f,     1, 0,  0, 0, 0,  0, 0, 0,  // 7

        -0.5f,  0.5f,  0.5f, 0.6f, 0.8f, 0.2f,  -1.0f,  0.0f,  0.0f,     1, 1,  0, 0, 0,  0, 0, 0,  // 8 (重复 0)
        -0.5f,  0.5f, -0.5f, 0.6f, 0.8f, 0.2f,  -1.0f,  0.0f,  0.0f,     0, 1,  0, 0, 0,  0, 0, 0,  // 9 (重复 4)
        -0.5f, -0.5f, -0.5f, 0.6f, 0.8f, 0.2f,  -1.0f,  0.0f,  0.0f,     0, 0,  0, 0, 0,  0, 0, 0,  // 10 (重复 7)
        -0.5f, -0.5f,  0.5f, 0.6f, 0.8f, 0.2f,  -1.0f,  0.0f,  0.0f,     1, 0,  0, 0, 0,  0, 0, 0,  // 11 (重复 3)

        0.5f,  0.5f,  0.5f,  0.9f, 0.3f, 0.6f,   1.0f,  0.0f,  0.0f,     0, 1,  0, 0, 0,  0, 0, 0,    // 12 (重复 1)
        0.5f,  0.5f, -0.5f,  0.9f, 0.3f, 0.6f,   1.0f,  0.0f,  0.0f,     1, 1,  0, 0, 0,  0, 0, 0,    // 13 (重复 5)
        0.5f, -0.5f, -0.5f,  0.9f, 0.3f, 0.6f,   1.0f,  0.0f,  0.0f,     1, 0,  0, 0, 0,  0, 0, 0,    // 14 (重复 6)
        0.5f, -0.5f,  0.5f,  0.9f, 0.3f, 0.6f,   1.0f,  0.0f,  0.0f,     0, 0,  0, 0, 0,  0, 0, 0,    // 15 (重复 2)

        -0.5f,  0.5f,  0.5f, 0.4f, 0.5f, 0.6f,   0.0f,  1.0f,  0.0f,     0, 0,  0, 0, 0,  0, 0, 0, // 16 (重复 0)
        0.5f,  0.5f,  0.5f,  0.4f, 0.5f, 0.6f,   0.0f,  1.0f,  0.0f,     1, 0,  0, 0, 0,  0, 0, 0,  // 17 (重复 1)
        0.5f,  0.5f, -0.5f,  0.4f, 0.5f, 0.6f,   0.0f,  1.0f,  0.0f,     1, 1,  0, 0, 0,  0, 0, 0,  // 18 (重复 5)
        -0.5f,  0.5f, -0.5f, 0.4f, 0.5f, 0.6f,   0.0f,  1.0f,  0.0f,     0, 1,  0, 0, 0,  0, 0, 0,  // 19 (重复 4)

        -0.5f, -0.5f,  0.5f, 0.6f, 0.3f, 0.5f,   0.0f, -1.0f,  0.0f,     0, 0,  0, 0, 0,  0, 0, 0,  // 20 (重复 3)
        0.5f, -0.5f,  0.5f,  0.6f, 0.3f, 0.5f,   0.0f, -1.0f,  0.0f,     1, 0,  0, 0, 0,  0, 0, 0,   // 21 (重复 2)
        0.5f, -0.5f, -0.5f,  0.6f, 0.3f, 0.5f,   0.0f, -1.0f,  0.0f,     1, 1,  0, 0, 0,  0, 0, 0,   // 22 (重复 6)
        -0.5f, -0.5f, -0.5f, 0.6f, 0.3f, 0.5f,   0.0f, -1.0f,  0.0f,     0, 1,  0, 0, 0,  0, 0, 0,   // 23 (重复 7)
    };

    GLuint indices[] = {
        // 前面 (Front Face)
        0, 1, 2, 2, 3, 0,

        // 后面 (Back Face)
        4, 5, 6, 6, 7, 4,

        // 左面 (Left Face)
        8, 9, 10, 10, 11, 8,

        // 右面 (Right Face)
        12, 13, 14, 14, 15, 12,

        // 上面 (Top Face)
        16, 17, 18, 18, 19, 16,

        // 下面 (Bottom Face)
        20, 21, 22, 22, 23, 20,
    };
    calculateTangentBiTangent(vertices, indices, 24, 36);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

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
    m_model = identity<mat4>();
    // m_view = identity<mat4>();
    // m_projection = identity<mat4>();
    // m_view = translate(m_view, vec3(0.0f, 0.0f, -3.0f));
    // m_projection  = perspective(radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
    glBindVertexArray(0);
}

void cube::draw(GLuint currentProgram)
{
    //glUseProgram(currentProgram);
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

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    recordError("cube Draw Error: ");

    glBindVertexArray(0);
  //  glUseProgram(0);
}


