#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"
//#include "converttoshaderlight.h"
class DirectionalLight : public Light {
public:
    DirectionalLight(const QColor& lightColor = QColor(255, 255, 255), float lightIntensity = 10, const glm::vec3& lightDirection = glm::vec3(0.0f,0.0f,-1.0f));

    void setLightDirection(const glm::vec3& direction);
    glm::vec3 getLightDirection() const{return m_lightDirection;}

    void setAllAttribute(ShaderLight sl);

 //   QColor computeLightContribution(const glm::vec3& /*point*/, const glm::vec3& normal) const override;

 //   ShaderLight toShaderLight() const override{return convertToShaderLight(*this);}
private:
    glm::vec3 m_lightDirection = glm::vec3(0.0f,0.0f,-1.0f);
};

#endif // DIRECTIONALLIGHT_H
