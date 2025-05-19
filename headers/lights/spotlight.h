#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
//#include "converttoshaderlight.h"
class SpotLight : public Light {
public:
    SpotLight(const QColor& lightColor = QColor(255, 255, 255), float lightIntensity = 10, const glm::vec3& lightPosition = glm::vec3(0.0f, 0.0f, 3.0f), const glm::vec3& lightDirection = glm::vec3(0.0f, 0.0f, -1.0f), float innerConeAngle = 45.0f, float outerConeAngle = 60.0f);

    inline void setLightPosition(const glm::vec3& position) { m_lightPosition = position; }
    inline void setLightDirection(const glm::vec3& direction) { m_lightDirection = direction; }
    inline void setInnerConeAngle(float angle) { m_innerConeAngle = angle; }
    inline void setOuterConeAngle(float angle) { m_outerConeAngle = angle; }

    inline glm::vec3 getLightPosition()const {return m_lightPosition;}
    inline glm::vec3 getLightDirection()const {return m_lightDirection;}
    inline float getLightInnerConeAngle() const {return m_innerConeAngle;}
    inline float getLightOuterConeAngle() const {return m_outerConeAngle;}
    inline float getLightconstant() const{return m_constant;}
    inline float getLightlinear() const{return m_linear;}
    inline float getLightquadratic() const{return m_quadratic;}
    void setAllAttribute(ShaderLight sl);

  //  QColor computeLightContribution(const glm::vec3& point, const glm::vec3& normal) const override ;

  //  ShaderLight toShaderLight() const override{return convertToShaderLight(*this);}
private:
    glm::vec3 m_lightPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    float m_innerConeAngle;
    float m_outerConeAngle;

    float m_constant; // 衰减因子，常数项
    float m_linear;   // 衰减因子，线性项
    float m_quadratic;//二次项，这三项是内角外角之间的衰减参数
};

#endif // SPOTLIGHT_H
