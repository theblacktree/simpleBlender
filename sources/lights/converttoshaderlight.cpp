#include "converttoshaderlight.h"
// ConvertToShaderLight.cpp

// 实现通用模板函数的基础版本
template<typename T>
ShaderLight convertToShaderLight(const T& light) {
    ShaderLight shaderLight;
    initShaderLight(shaderLight);
    QColor color = light.getLightColor();
    glm::vec3 vecvolor (color.redF(), color.greenF(), color.blueF());
    shaderLight.color = glm::vec4(vecvolor, 1.0);
    shaderLight.intensity = light.getLightIntensity();
    shaderLight._padding1 = 0.0f;
    shaderLight._padding2 = 0.0f;
    shaderLight._padding3 = 0.0f;
    return shaderLight;
}

// 特化 PointLight 类型的模板函数
template<>
ShaderLight convertToShaderLight<PointLight>(const PointLight& light) {
    ShaderLight shaderLight = convertToShaderLight<Light>(light); // 调用基类版本填充公共字段
    shaderLight.type = 0; // 假设 0 表示点光源
    shaderLight.position = glm::vec4(light.getLightPosition(), 0.0f);
    shaderLight.constant = light.getLightconstant();
    shaderLight.linear = light.getLightlinear();
    shaderLight.quadratic = light.getLightquadratic();
    return shaderLight;
}

template<>
ShaderLight convertToShaderLight<DirectionalLight>(const DirectionalLight& light) {
    ShaderLight shaderLight = convertToShaderLight<Light>(light); // 调用基类版本填充公共字段
    shaderLight.type = 1; // 假设 1 表示方向光源
    shaderLight.direction = glm::vec4(light.getLightDirection(), 0.0f);
    return shaderLight;
}

template<>
ShaderLight convertToShaderLight<SpotLight>(const SpotLight& light) {    
    ShaderLight shaderLight = convertToShaderLight<Light>(light); // 调用基类版本填充公共字段
    shaderLight.type = 2; // 假设 2 表示方聚光光源
    shaderLight.direction = glm::vec4(light.getLightDirection(), 0.0f);
    shaderLight.position = glm::vec4(light.getLightPosition(), 1.0f);
    shaderLight.innerAngle = light.getLightInnerConeAngle();
    shaderLight.outerAngle = light.getLightOuterConeAngle();
    shaderLight.constant = light.getLightconstant();
    shaderLight.linear = light.getLightlinear();
    shaderLight.quadratic = light.getLightquadratic();
    return shaderLight;
}


void initShaderLight(ShaderLight &sl)
{
    sl.type = -1; // 默认为无光源
    sl.position = glm::vec4(0.0f, 0.0f, -2.0f, 0.0f);
    sl.direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    sl.color = glm::vec4(0.3f, 0.5f, 0.7f, 1.0f); // lan色
    sl.intensity = 0.0f;
    sl.constant = 0.0f;
    sl.linear = 0.0f;
    sl.quadratic = 0.0f;
    sl.areaSize = glm::vec2(0.0f, 0.0f);
    sl.innerAngle = 0.0f;
    sl.outerAngle = 0.0f;
    sl._padding1 = 0.0f;
    sl._padding2 = 0.0f;
    sl._padding3 = 0.0f;
}
