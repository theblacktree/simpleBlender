#include "light.h"
using namespace  std;
Light::Light(Type lightType, const QColor& lightColor, float lightIntensity)
    : m_lightType(lightType), m_lightColor(lightColor), m_lightIntensity(lightIntensity)
{

}

QString Light::getLightTypeName()
{
    switch(m_lightType)
    {
        case Type::Point: return "Point";
        case Type::Directional: return "Directional";
        case Type::Spot: return "Spot";
        default: return "Unknown";
    }
}
