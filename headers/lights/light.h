#ifndef LIGHT_H
#define LIGHT_H
//这是灯光基类，所有灯光都需要继承这个类
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <QColor>
#include <QVector3D>
#include <memory>
#include <string>
#include "toolFunc.h"

struct alignas(16) ShaderLight
{
    // 注意：必须完全匹配 GLSL 的 std140 布局
    int type;                // 偏移 0
    float intensity;         // 4
    float constant;         // 8
    float linear;           // 12
    glm::vec4 position;     // 16（对齐到 16）
    glm::vec4 direction;    // 32
    glm::vec4 color;        // 48
    float quadratic;        // 64
    float innerAngle;       // 68
    glm::vec2 areaSize;     // 72（对齐到 8）
    float outerAngle;       // 80
    // 填充到 96 字节（16 的倍数）
    float _padding1;        // 84
    float _padding2;        // 88
    float _padding3;        // 92
};
    enum class Type { Point, Directional, Spot };
class Light {
public:


    Light(Type lightType, const QColor& lightColor = QColor(255, 255, 255), float lightIntensity = 30);

    virtual ~Light() = default;

    inline void setLightColor(const QColor& color){m_lightColor = color;}
    inline void setLightIntensity(float intensity){m_lightIntensity = intensity;}

    inline Type getLightType(){ return m_lightType;}
    QString getLightTypeName();
    inline QColor getLightColor() const { return m_lightColor; }
    inline float getLightIntensity() const { return m_lightIntensity; }

    virtual void setAllAttribute(ShaderLight sl) = 0;

protected:
    Type m_lightType;
    QColor m_lightColor;
    float m_lightIntensity;
};

#endif // LIGHT_H
