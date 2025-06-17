#include "mesh.h"
using namespace std;
Mesh::Mesh(std::vector<Mesh::Vertex> vertices, vector<unsigned int> indices, std::vector<Mesh::AssimpTexture> textures)
{
    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;
    /*在这里处理加载的纹理数据，因为纵使我知道每种纹理的数量，但是将不定量的纹理上传到着色器中并应用也是一件比较值得仔细考虑的事情
暂时只输入一种纹理*/

}

void Mesh::initialize()
{
    for (int i = 0; i<m_textures.size(); ++i)
    {
        if (m_textures[i].type == TextureType::DIFFUSE_MAP)
        {
            m_isUseDiffuseMap = true;
            m_diffuseTextureId = m_textures[i].id;
        }
        else if (m_textures[i].type == TextureType::NORMAL_MAP)
        {
            m_isUseNormalMap = true;
            m_normalTextureId = m_textures[i].id;
        }
        else if (m_textures[i].type == TextureType::SPECULAR_MAP)
        {
            specularTextureId = m_textures[i].id;
        }
        else if (m_textures[i].type == TextureType::ROUGHNESS_MAP)
        {
            m_roughnessTextureId = m_textures[i].id;
        }
        else//AO贴图
        {
            m_aoTextureId = m_textures[i].id;
        }
    }
    for (int i = 0; i<m_textures.size(); ++i)
    {

    }
    initializeOpenGLFunctions();
    // 确认存在有效的 OpenGL 上下文
    glCullFace(GL_BACK);  // 剔除背面
    glFrontFace(GL_CCW);  // 逆时针顺序为正面
    if (!QOpenGLContext::currentContext())
    {
        qWarning() << "No current OpenGL context in Cube::initialize";
        return;
    }
    recordError("mesh Draw Error line33 : ");
    computeTangents(m_vertices, m_indices);
    recordError("mesh Draw Error line35 : ");
    // 确保 OpenGL 上下文是当前的
    if (!QOpenGLContext::currentContext()->extraFunctions())
    {
        qCritical("OpenGL functions are not initialized.");
        return;
    }
    recordError("mesh Draw Error line42 : ");
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    recordError("mesh Draw Error line45 : ");
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    recordError("mesh Draw Error line49: ");
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW );
    recordError("mesh Draw Error line53 : ");
    recordError("buffer Error: ");
    //顶点坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    //颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //法线坐标
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //uv纹理坐标
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);
    //顶点切线
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(4);
    //顶点副切线
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(15 * sizeof(float)));
    glEnableVertexAttribArray(5);
    //变换矩阵
    recordError("mesh Draw Error line74 : ");
    m_model = glm::identity<glm::mat4>();
    glBindVertexArray(0);
}

void Mesh::draw(GLuint currentProgram)
{
    initialize();
    recordError("mesh Draw Error line74 : ");
    glBindVertexArray(m_vao);
    recordError("mesh paintGL bind Error: ");
    //对模型进行缩小
    //m_model = glm::scale(m_model,glm::vec3(0.1f));
    GLint modelLoc = glGetUniformLocation(currentProgram, "amodel");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_model));
    recordError("mesh modelLoc Draw Error line : ");
    GLint objectColorLoc = glGetUniformLocation(currentProgram, "objectColor");
    glUniform4fv(objectColorLoc, 1, glm::value_ptr(m_color));
    recordError("mesh objcolorLoc Draw Error line : ");
    GLint useObjectColorLoc = glGetUniformLocation(currentProgram, "useObjectColor");
    glUniform1i(useObjectColorLoc, m_isUseObjectColor ? 1 : 0);
    recordError("mesh useObjectColorLoc Draw Error line : ");
    GLint metallicLoc = glGetUniformLocation(currentProgram, "metallic");
    glUniform1f(metallicLoc, m_metallic);
    GLint roughnessLoc = glGetUniformLocation(currentProgram, "roughness");
    glUniform1f(roughnessLoc, m_roughness);
    GLint iorLoc = glGetUniformLocation(currentProgram, "ior");
    glUniform1f(iorLoc, m_ior);
    recordError("mesh metallicLoc Draw Error line : ");
    //texture variety
    GLint useDiffuseMapLoc = glGetUniformLocation(currentProgram, "isUseDiffuseMap");
    glUniform1i(useDiffuseMapLoc, m_diffuseTextureId ? 1 : 0);
    GLint useUseNormalMapLoc = glGetUniformLocation(currentProgram, "isUseNormalMap");
    glUniform1i(useUseNormalMapLoc, m_normalTextureId ? 1 : 0);
    GLint useCubeMapLoc = glGetUniformLocation(currentProgram, "isUseCubeMap");
    glUniform1i(useCubeMapLoc, m_cubeTextureId ? 1 : 0);
    //follow is bing texture
    recordError("mesh textureload Draw Error line : ");
    // bind Texture
    // Activate and bind the diffuse texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_diffuseTextureId);

    // Activate and bind the normal texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalTextureId);
    recordError("mesh textureload Draw Error line110 : ");
    // Activate and bind the specular texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTextureId);
    recordError("mesh textureload Draw Error line : ");
    //粗糙度
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_roughnessTextureId);
    recordError("mesh textureload Draw Error line : ");
    //AO
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_aoTextureId);
    recordError("mesh textureload Draw Error line : ");
    //高光贴图
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, specularTextureId);
    recordError("mesh textureload Draw Error line : ");
    // Assuming you have a shader program that expects these textures in specific texture units
    glUniform1i(glGetUniformLocation(currentProgram, "diffuseMap"), 0); // GL_TEXTURE0
    glUniform1i(glGetUniformLocation(currentProgram, "normalMap"), 1);   // GL_TEXTURE1
    glUniform1i(glGetUniformLocation(currentProgram, "cubeMap"), 2); // GL_TEXTURE2
    glUniform1i(glGetUniformLocation(currentProgram, "roughnessMap"), 3);   // GL_TEXTURE3
    glUniform1i(glGetUniformLocation(currentProgram, "aoMap"), 4);   // GL_TEXTURE4
    glUniform1i(glGetUniformLocation(currentProgram, "specularMap"), 5);   // GL_TEXTURE5

    recordError("mesh textureload Draw Error line121 : ");
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
    recordError("mesh glDrawElements Error line : ");

    glBindVertexArray(0);
}
