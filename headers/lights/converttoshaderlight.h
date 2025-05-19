#ifndef CONVERTTOSHADERLIGHT_H
#define CONVERTTOSHADERLIGHT_H
#include "light.h"
#include "pointlight.h"
#include "directionallight.h"
#include "spotlight.h"
// 声明通用模板函数
template<typename T>
ShaderLight convertToShaderLight(const T& light);

template<>
ShaderLight convertToShaderLight<PointLight>(const PointLight& light);

template<>
ShaderLight convertToShaderLight<DirectionalLight>(const DirectionalLight& light);

template<>
ShaderLight convertToShaderLight<SpotLight>(const SpotLight& light);

void initShaderLight(ShaderLight& sl);

#endif // CONVERTTOSHADERLIGHT_H
