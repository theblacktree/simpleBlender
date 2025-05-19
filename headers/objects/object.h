#ifndef OBJECT_H
#define OBJECT_H
//这是物体基类，所有物体都需要继承这个类
#include <GL/glew.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <map>
#include <algorithm>
#include <qvectornd.h>
#include <string>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <fstream>
#include <sstream>
#include "toolfunc.h"
#include "texturemanager.h"
#include "logger.h"
class Object: public QOpenGLExtraFunctions
{
public:
    Object();

    virtual ~Object() {}

    struct Vertex
    {
        float x, y, z; // 位置
        float r, g, b;// a; 颜色
        float nx, ny, nz; // 法线
        float u, v; // UV坐标
        float tangentX, tangentY, tangentZ; // 切线
        float bitangentX, bitangentY, bitangentZ; // 副切线
    };

    // 初始化物体（加载着色器、缓冲区等）
    virtual void initialize() = 0;

    // 绘制物体
    virtual void draw(GLuint currentProgram) = 0;

    // 更新物体状态（例如动画、物理模拟等）
    virtual void update(float deltaTime) {}

    // 获取模型矩阵
    const glm::mat4& getModelMatrix() const { return m_model; }

    // 设置模型矩阵
    void setModelMatrix( glm::vec3& translationXYZ, glm::vec3& rotationXYZ, glm::vec3& scaleXYZ);

    // 获取位置
    inline glm::vec3 getPosition() {return m_position;}

    // 获取旋转
    inline glm::vec3 getRotation() {return m_rotation; }

    // 获取缩放
    inline glm::vec3 getScale() {return m_scale;}

    void setObjectColor(QColor &color);

    void setMaterialValue(float metallic, float roughness, float ior);

    std::vector<float> getMaterialValue();

    void recordError(const char* s);

    inline void setDiffuseTexture(GLuint textureId) {m_diffuseTextureId = textureId;}

    inline void setNormalTexture(GLuint textureId) {m_normalTextureId = textureId;}

    inline void setCubeTexture(GLuint textureId) {m_cubeTextureId = textureId;}
    //传入数组
    void calculateTangentBiTangent(GLfloat vertices[], GLuint indices[], size_t verticesRowCount, size_t indicesSize);
    //传入vector
    void computeTangents(std::vector<Object::Vertex>& vertices, const std::vector<unsigned int>& indices);
    void removeUVTexture(TextureType textureType);
protected:

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    glm::mat4 m_model = glm::mat4(1.0f); // 模型矩阵

   //鼠标拖动事件相关
    // bool m_isDragging;
    // QPoint m_lastPoint;
    // float m_rotateX;
    // float m_rotateY;
    // float m_rotateZ;
    //设置颜色相关
    glm::vec4 m_color;
    bool m_isUseObjectColor = false;

    //平移旋转缩放
    glm::vec3 m_position;//位置就是平移
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    //设置材质值相关，金属度，糙度，折射率
    float m_metallic;
    float m_roughness;
    float m_ior;

    //Textureid
    GLuint m_diffuseTextureId = 0;   // Diffuse map texture ID
    GLuint m_normalTextureId = 0;    // Normal map texture ID
    GLuint m_cubeTextureId = 0;  // Specular map texture ID

    bool m_isUseDiffuseMap = false;   // 是否有漫反射贴图
    bool m_isUseNormalMap = false;    // 是否有法线贴图
    bool m_isUseCubeMap = false;      // 是否有立方体贴图



};

#endif // OBJECT_H
