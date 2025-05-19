#ifndef TOOLFUNC_H
#define TOOLFUNC_H
#include <iostream>
#include <QMatrix4x4>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "singleton.h"
class toolFunc:public Singleton<toolFunc>
{
public:
    // toolFunc( toolFunc& t) = delete;
    // toolFunc& operator=( toolFunc& t) = delete;
    // static toolFunc& GetInstance()
    // {
    //     static toolFunc tf;
    //     return tf;
    // }
    QMatrix4x4 convertGlmToQMatrix(const glm::mat4& glmMatrix);
    glm::vec4 qColor4ToVec4(QColor& color);
private:
    // toolFunc();
    // ~toolFunc();
};

#endif // TOOLFUNC_H
