#include "directionallight.h"

DirectionalLight::DirectionalLight(const QColor& lightColor, float lightIntensity, const glm::vec3& lightDirection)
    : Light(Type::Directional, lightColor, lightIntensity), m_lightDirection(lightDirection)
{

}
void DirectionalLight::setLightDirection(const glm::vec3& direction)
{
    m_lightDirection = direction;
}

void DirectionalLight::setAllAttribute(ShaderLight sl)
{
    m_lightIntensity = sl.intensity;
    m_lightDirection.x = sl.direction.x;
    m_lightDirection.y = sl.direction.y;
    m_lightDirection.z = sl.direction.z;
}
/*QColor DirectionalLight::computeLightContribution(const glm::vec3& , const QVector3D& normal) const
{
    // 归一化方向向量
    QVector3D normalizedDirection = m_lightDirection.normalized();

    // 计算法线与光线方向的点积
    float nDotL = std::max(0.0f, QVector3D::dotProduct(normal, normalizedDirection));

    // 返回颜色贡献
    QColor resultColor = m_lightColor;
    resultColor.setRedF(resultColor.redF() * m_lightIntensity * nDotL);
    resultColor.setGreenF(resultColor.greenF() * m_lightIntensity * nDotL);
    resultColor.setBlueF(resultColor.blueF() * m_lightIntensity * nDotL);
    return resultColor;
}*/
