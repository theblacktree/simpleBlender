#ifndef HDROBJECT_H
#define HDROBJECT_H
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

class HDRObject: public QOpenGLExtraFunctions
{
public:
    HDRObject();
    ~HDRObject();
    // 初始化物体（加载着色器、缓冲区等）
    void initialize();
    // 设置模型矩阵
    void setModelMatrix(glm::mat4 & model);

    // 绘制物体
    void draw(GLuint currentProgram);

    void recordError(const char* s);

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    glm::mat4 m_model = glm::mat4(1.0f); // 模型矩阵
};

#endif
