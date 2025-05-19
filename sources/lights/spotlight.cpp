#include "spotlight.h"

SpotLight::SpotLight(const QColor& lightColor, float lightIntensity, const glm::vec3& lightPosition, const glm::vec3& lightDirection, float innerConeAngle, float outerConeAngle)
    : Light(Type::Spot, lightColor, lightIntensity),
    m_lightPosition(lightPosition),
    m_lightDirection(lightDirection),
    m_innerConeAngle(innerConeAngle),
    m_outerConeAngle(outerConeAngle)
{

}

void SpotLight::setAllAttribute(ShaderLight sl)
{
    m_lightIntensity = sl.intensity;
    m_lightPosition.x = sl.position.x;
    m_lightPosition.y = sl.position.y;
    m_lightPosition.z = sl.position.z;
    m_lightDirection.x = sl.direction.x;
    m_lightDirection.y = sl.direction.y;
    m_lightDirection.z = sl.direction.z;
    m_innerConeAngle = sl.innerAngle;
    m_outerConeAngle = sl.outerAngle;

    m_quadratic = sl.quadratic;
    m_linear = sl.linear;
    m_constant = sl.constant;
}

/*QColor SpotLight::computeLightContribution(const QVector3D& point, const QVector3D& normal) const {
    // 计算光的方向向量
    QVector3D lightDirectionToPoint = point - m_lightPosition;
    lightDirectionToPoint.normalize();

    // 计算角度衰减因子
    QVector3D normalizedLightDirection = m_lightDirection.normalized();
    float cosTheta = QVector3D::dotProduct(-lightDirectionToPoint, normalizedLightDirection);
    float epsilon = m_innerConeAngle - m_outerConeAngle;
    float intensity = std::clamp((cosTheta - m_outerConeAngle) / epsilon, 0.0f, 1.0f);

    if (intensity <= 0.0f) return QColor(0, 0, 0); // 如果不在聚光灯范围内，返回黑色

    // 计算法线与光线方向的点积
    float nDotL = std::max(0.0f, QVector3D::dotProduct(normal, -lightDirectionToPoint));

    // 返回颜色贡献
    QColor resultColor = m_lightColor;
    resultColor.setRedF(resultColor.redF() * m_lightIntensity * nDotL * intensity);
    resultColor.setGreenF(resultColor.greenF() * m_lightIntensity * nDotL * intensity);
    resultColor.setBlueF(resultColor.blueF() * m_lightIntensity * nDotL * intensity);
    return resultColor;
}
*/
