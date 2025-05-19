#include "pointlight.h"
using namespace glm;
using namespace std;
PointLight::PointLight(const QColor& lightColor, float lightIntensity, const vec3& lightPosition)
    : Light(Type::Point, lightColor, lightIntensity), m_lightPosition(lightPosition)
{
    m_constant = 1.0f;
    m_linear = 0.09f;
     m_quadratic = 0.032f;
}

void PointLight::setAllAttribute(ShaderLight sl)
{
    m_lightPosition.x = sl.position.x;
    m_lightPosition.y = sl.position.y;
    m_lightPosition.z = sl.position.z;
    m_lightIntensity = sl.intensity;
    m_quadratic = sl.quadratic;
    m_linear = sl.linear;
    m_constant = sl.constant;
}

