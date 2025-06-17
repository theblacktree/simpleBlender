#version 430 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in mat3 outTBN;
struct ShaderLight
{
    int type;            // 4 字节（对齐到 4）
    float intensity;     // 4 字节（对齐到 4）
    float constant;      // 4 字节（对齐到 4）
    float linear;        // 4 字节（对齐到 4）

    vec4 position;       // 16 字节（对齐到 16）
    vec4 direction;      // 16 字节（对齐到 16）
    vec4 color;          // 16 字节（对齐到 16）

    float quadratic;     // 4 字节（对齐到 4）
    float innerAngle;    // 4 字节（对齐到 4）
    vec2 areaSize;       // 8 字节（对齐到 8）

    float outerAngle;    // 4 字节（对齐到 4）
    // 填充 12 字节（确保结构体大小是 16 的倍数）
    float _padding1;      //4 字节（手动填充）
    float _padding2;      //4 字节（手动填充）
    float _padding3;      //4 字节（手动填充）
};

layout(std140, binding = 0) uniform LightBlock
{
    ShaderLight lights[50];

};
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D normalMap;

uniform bool isUseDiffuseMap;   // 是否有漫反射贴图
uniform bool isUseNormalMap;    // 是否有法线贴图
uniform int numLights;
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
    return normalize(tangentNormal);
}

void main()
{
    vec3 TangentLightPos = outTBN * lights[0].position.xyz;
    // 获取法线、光照方向、视线方向
    vec3 norm = getNormalFromMap();
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    // 半程向量（用于Blinn-Phong）
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // 漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(diffuseMap, TexCoords).rgb;

    // 高光
    float spec = pow(max(dot(norm, halfwayDir), 0.0), texture(specularMap, TexCoords).r * 256.0); // 使用贴图中的值作为高光指数
    vec3 specular = texture(specularMap, TexCoords).rgb * spec * lights[0].color.rgb;

    // 粗糙度影响高光强度（可选）
    float roughness = texture(roughnessMap, TexCoords).r;
    spec *= (1.0 - roughness); // 粗糙度越高，高光越弱
    specular *= spec;

    // 环境光遮挡
    float ao = texture(aoMap, TexCoords).r;

    // 最终颜色
    // 计算最终颜色
       // vec3 ambient = 0.2 *lights[0].color.rgb * lights[0].intensity * texture(diffuseMap, TexCoords).rgb * ao; // 假设环境光是光源颜色的十分之一
        vec3 ambient = vec3(0.1) * texture(diffuseMap, TexCoords).rgb * ao;
        vec3 result = ambient + (diffuse + specular)*lights[0].intensity;
        //色调映射使Lo从LDR的值映射为HDR的值
        result = result / (result + vec3(1.0));
        result = pow(result, vec3(1.0/2.2));
        // Gamma 校正
        result = pow(result, vec3(1.0 / 2.2));
        FragColor = vec4(result, 1.0);
}
