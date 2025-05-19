#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "Light.h"

//这是点光源的类
class PointLight : public Light {
public:
    PointLight(const QColor& lightColor = QColor(255, 255, 255), float lightIntensity = 10, const glm::vec3& lightPosition = glm::vec3(0.0f,0.0f,3.0f));

    inline void setLightPosition(const glm::vec3& position){m_lightPosition = position;}
    inline void setLightconstant(const float& constant){m_constant = constant;}
    inline void setLightlinear(const float& linear){m_linear = linear;}
    inline void setLightquadratic(const float& quadratic){m_quadratic = quadratic;}

    inline glm::vec3 getLightPosition() const{return m_lightPosition;}
    inline float getLightconstant() const{return m_constant;}
    inline float getLightlinear() const{return m_linear;}
    inline float getLightquadratic() const{return m_quadratic;}

    void setAllAttribute(ShaderLight sl);
  //  QColor computeLightContribution(const glm::vec3& point, const glm::vec3& normal) const override ;
private:
    glm::vec3 m_lightPosition = glm::vec3(0.0f,0.0f,3.0f);
    float m_constant; // 衰减因子（仅对点光源有效），常数项
    float m_linear;   // 衰减因子（仅对点光源有效），线性项
    float m_quadratic; // 衰减因子（仅对点光源有效），二次项
};

#endif // POINTLIGHT_H
