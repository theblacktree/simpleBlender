#include "toolfunc.h"
#include <qcolor.h>
using namespace   glm;
using namespace std;
//toolFunc::toolFunc() {}


//toolFunc::~toolFunc(){}

QMatrix4x4 toolFunc::convertGlmToQMatrix(const mat4& glmMatrix) {
    QMatrix4x4 qMatrix;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            qMatrix(i, j) = glmMatrix[j][i];
        }
    }
    return qMatrix;
}

vec4 toolFunc::qColor4ToVec4(QColor& color)
{
    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();
    float a = color.alphaF();
    vec4 v(r, g, b, a);
    return v;
}
