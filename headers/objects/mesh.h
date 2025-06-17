#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>
#include <QObject>
#include <QWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLContext>
#include <QMatrix4x4>
#include <QTimer>
#include <QMouseEvent>
#include <QFile>
#include "object.h"
#include "toolFunc.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp" // 包含矩阵变换函数
#include <vector>

class Mesh:public Object
{  
public:
    //构造时接收解析模型的网格数据传入
    Mesh(std::vector<Mesh::Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh::AssimpTexture> textures);
    void initialize()override;

    void draw(GLuint currentProgram)override;
private:
    /*  网格数据  */
    std::vector<Mesh::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Mesh::AssimpTexture> m_textures;
    GLuint m_roughnessTextureId = 0;   // roughness map texture ID
    GLuint m_aoTextureId = 0;    // AO map texture ID
    GLuint specularTextureId = 0;//高光贴图
};

#endif
