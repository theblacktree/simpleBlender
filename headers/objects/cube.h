#ifndef CUBE_H
#define CUBE_H
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

class cube: public Object
{
public:
    cube();
    ~cube();
    void initialize()override;

    void draw(GLuint currentProgram)override;

private:
    QString vertexPath = ":/cube/vertex.vert";
    QString fragmentPath = ":/cube/frag.frag";

};

#endif // cube_H
