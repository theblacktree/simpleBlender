#version 430 core

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

in vec3 fragPos;      // 片元的世界坐标
in vec3 normal;       // 法线向量
in vec3 viewDir;       //视线向量
in vec4 aOutColor;    //传递的颜色
in vec2 texCoord;      //UV贴图向量
in mat3 TBN;          //TBN矩阵
out vec4 FragColor;    //最终输出颜色

// 贴图采样器
uniform sampler2D diffuseMap; // 漫反射贴图
uniform sampler2D normalMap;  // 法线贴图
uniform samplerCube cubeMap;  // 立方体贴图
uniform sampler2D HDRMap;//hdr贴图equirectangularMap，一般是球形映射
// 贴图控制变量
uniform bool isUseDiffuseMap;   // 是否有漫反射贴图
uniform bool isUseNormalMap;    // 是否有法线贴图
uniform bool isUseCubeMap;      // 是否有立方体贴图
uniform bool isUseHDRMap;       // 是否有hdr贴图

uniform int numLights;
uniform vec4 objectColor; // 包含透明度的颜色
uniform bool useObjectColor; // 控制使用哪种颜色的标志位
uniform float metallic;// 金属度
uniform float roughness;// 粗糙度
uniform float ior;// 折射率（IOR）
const float PI = 3.14159265359;

// 计算法线分布函数（GGX）
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 1e-5);
}

// 几何遮挡项（Schlick-GGX）
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 1e-5);
}

// 几何遮挡项（Smith 方法）
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// 菲涅尔方程（Schlick 近似）
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(void)
{
    vec3 N;
    if (isUseNormalMap == false)
        N = normalize(normal); // 法线向量
    else
    {
    // 从法线贴图中采样法线（范围从[0, 1]映射到[-1, 1]）
        N = texture(normalMap, texCoord).rgb;
        N = normalize(N * 2.0 - 1.0); // 将法线从切线空间转换到世界空间

        // 使用TBN矩阵将法线从切线空间转换到世界空间
        N = normalize(TBN * N);
    }
    vec3 V = viewDir; // 视线向量（假设相机在原点）

    vec4 baseColor = useObjectColor ? objectColor : aOutColor;
   // vec4 baseColor = vec4(0.8, 0.5, 0.2, 1.0f);//铜色
    if (isUseDiffuseMap) {
        baseColor *= texture(diffuseMap, texCoord); // Sample the diffuse map
    }

    // 基础反射率
    vec3 F0 = vec3(0.04); // 非金属的基础反射率
    F0 = mix(F0, baseColor.rgb, metallic);
    vec3 Lo = vec3(0.0); // 最终光照结果

    for (int i = 0; i < min(numLights, 50); ++i)
    {
        vec3 L; // 光源方向
        float attenuation = 1.0; // 衰减因子
        float spotEffect = 1.0;  // 聚光灯效果因子，默认为1（无影响）

        if (lights[i].type == 1)
        { // 方向光
            L = normalize(-lights[i].direction.xyz);
        } else if (lights[i].type == 0)
        { // 点光源
            vec3 lightDir = lights[i].position.xyz - fragPos;
            float distance = max(length(lightDir), 0.001);
            L = normalize(lightDir);
            attenuation = 1.0 / (lights[i].constant +
                                 lights[i].linear * distance +
                                 lights[i].quadratic * distance * distance);
        }
        else if (lights[i].type == 2)
        {
            // 聚光灯：结合方向和位置，模拟手电筒效果
            vec3 lightDir = normalize(lights[i].position.xyz - fragPos); // 片段到光源的方向
            vec3 spotDir = normalize(-lights[i].direction.xyz);          // 聚光灯的方向

            // 计算当前片段与聚光灯方向的夹角余弦值
            float cosTheta = dot(lightDir, spotDir);

            // 聚光灯的内外圆锥角（以余弦值表示）
            float outerConeCos = cos(radians(lights[i].outerAngle)); // 外圆锥角余弦值
            float innerConeCos = cos(radians(lights[i].innerAngle)); // 内圆锥角余弦值

            // 聚光灯效果：平滑过渡（在内外圆锥角之间）
            spotEffect = clamp((cosTheta - outerConeCos) / (innerConeCos - outerConeCos), 0.0, 1.0);

            // 如果片段在聚光灯的外圆锥角之外，则不贡献光照
            if (cosTheta < outerConeCos)
            {
                attenuation = 0.0; // 完全无光照
            }
            else
            {
                // 聚光灯的衰减计算
                float distance = length(lights[i].position.xyz - fragPos);
                attenuation = 1.0 / (lights[i].constant +
                                     lights[i].linear * distance +
                                     lights[i].quadratic * distance * distance);
            }

            L = lightDir; // 使用片段到光源的方向作为光照方向
        }
        else
        {
            // 默认处理未知光源类型
            L = vec3(0.0, 0.0, -1.0); // 默认方向向量
            attenuation = 1.0;        // 默认无光照贡献
        }

        // 半角向量
        vec3 H = normalize(V + L);

        // 法线分布函数
        float D = DistributionGGX(N, H, roughness);

        // 几何遮挡项
        float G = GeometrySmith(N, V, L, roughness);

        // 菲涅尔方程
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        // 计算镜面反射项
        vec3 numerator = D * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 1e-5);

        // 漫反射项
        vec3 kD = vec3(1.0) - F; // 能量守恒
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        vec3 diffuse = kD * baseColor.rgb / PI;

        // 结合漫反射和镜面反射
        vec3 radiance = lights[i].color.xyz * lights[i].intensity * attenuation * spotEffect;
        Lo += (diffuse + specular) * radiance * NdotL;
    }

    // 环境光（可选）
    vec3 ambient = vec3(0.1) * baseColor.rgb;

    // 最终颜色
    vec3 color = ambient + Lo;

    // Gamma 校正
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, baseColor.a);

    // vec3 normal = texture(normalMap, texCoord).rgb;
    // FragColor = vec4(normal, 1.0); // 直接输出法线贴图的颜色
   // FragColor = vec4(texCoord, 0.0, 1.0); // 输出纹理坐标为颜色
    //FragColor = vec4(TBN[0], 1.0); // 输出切线 (T)
    //FragColor = vec4(TBN[1], 1.0); // 输出副切线 (B)
   // FragColor = vec4(TBN[2], 1.0); // 输出法线 (N)
    //FragColor = vec4(TBN[0] * 0.5 + 0.5, 1.0); // 将切线方向映射到 [0, 1] 范围并输出
   // FragColor = vec4(TBN[1] * 0.5 + 0.5, 1.0); // 将副切线方向映射到 [0, 1] 范围并输出
    //vec3 no = texture(normalMap, texCoord).rgb;
    //FragColor = vec4(no +ambient, 1.0); // 直接输出法线贴图的颜色
}
