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

in VS_OUT {
vec3 fragPos;      // 片元的世界坐标
vec4 aOutColor;    //传递的颜色
vec3 normal;       // 法线向量
vec2 texCoord;      //UV贴图向量
vec3 viewDir;       //视线向量
mat3 TBN;          //TBN矩阵
}fs_in;
out vec4 FragColor;    //最终输出颜色

// 贴图采样器
uniform sampler2D diffuseMap; // 漫反射贴图
uniform sampler2D normalMap;  // 法线贴图
uniform samplerCube cubeMap;  // 立方体贴图
//uniform sampler2D HDRMap;//hdr贴图equirectangularMap，一般是球形映射

// 贴图控制变量
uniform bool isUseDiffuseMap;   // 是否有漫反射贴图
uniform bool isUseNormalMap;    // 是否有法线贴图
uniform bool isUseCubeMap = false;      // 是否有立方体贴图,默认是没有的，只有在立方体物体并且将该值置为true才有用
//uniform bool isUseHDRMap;       // 是否有hdr贴图

uniform int numLights = 0;
uniform vec4 objectColor; // 包含透明度的颜色
uniform bool useObjectColor; // 控制使用哪种颜色的标志位
uniform float metallic;// 金属度
uniform float roughness;// 粗糙度
uniform float ior;// 折射率（IOR）
const float PI = 3.14159265359;

uniform float far_plane;//远裁剪面距离，用于阴影计算
uniform samplerCube depthMap;

/*法线分布函数，几何遮挡函数，菲涅尔方程是计算反射光的*/
// 计算法线分布函数（GGX）D估算在受到表面粗糙度的影响下，朝向方向与半程向量一致的微平面的数量。
//这是用来估算微平面的主要函数。
float DistributionGGX(vec3 N, vec3 H, float roughness)
{//和learnopengl Trowbridge-Reitz GGX一致
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
    float k = (r * r) / 8.0;//针对直接光照的重映射

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 1e-5);
}

// 几何遮挡项（Smith 方法）G描述了微平面自成阴影的属性。当一个平面相对比较粗糙的时候，
//平面表面上的微平面有可能挡住其他的微平面从而减少表面所反射的光线。
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// 菲涅尔方程（Schlick 近似）F菲涅尔方程描述的是在不同的表面角下表面所反射的光线所占的比率
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{//F0和金属度有关系，learnopengl写法，保证了金属和非金属的反射比率问题
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);//注意这里用的clamp是为了避免黑点
}
//计算阴影
// float ShadowCalculation(vec3 fragPos)
// {
//     if (numLights == 0)
//     {
//         return 0.0;
//     }
//     // Get vector between fragment position and light position
//     vec3 fragToLight = fragPos - lights[0].position.xyz;
//     // Use the light to fragment vector to sample from the depth map
//     float closestDepth = texture(depthMap, fragToLight).r;
//     // It is currently in linear range between [0,1]. Re-transform back to original value
//     closestDepth *= far_plane;
//     // Now get current linear depth as the length between the fragment and light position
//     float currentDepth = length(fragToLight);
//     // Now test for shadows
//     float bias = 0.05;
//     float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

//     return shadow;
// }
void main(void)
{
    vec3 N;
    if (isUseNormalMap == false)
        N = normalize(fs_in.normal); // 法线向量
    else
    {
    // 从法线贴图中采样法线（范围从[0, 1]映射到[-1, 1]）
        N = texture(normalMap, fs_in.texCoord).rgb;
        N = normalize(N * 2.0 - 1.0); // 将法线从切线空间转换到世界空间

        // 使用TBN矩阵将法线从切线空间转换到世界空间
        N = normalize(fs_in.TBN * N);
    }
    vec3 V = fs_in.viewDir; // 视线向量（假设相机在原点）

    vec4 baseColor = useObjectColor ? objectColor : fs_in.aOutColor;
   // vec4 baseColor = vec4(0.8, 0.5, 0.2, 1.0f);//铜色
    if (isUseDiffuseMap)
    {
        baseColor *= texture(diffuseMap, fs_in.texCoord); // Sample the diffuse map
    }
    if (isUseCubeMap)
    {
        baseColor = texture(cubeMap, fs_in.fragPos);
    }

    // 基础反射率
    vec3 F0 = vec3(0.04); // 非金属的基础反射率
    F0 = mix(F0, baseColor.rgb, metallic);//F0用来计算菲涅尔方程
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
            vec3 lightDir = lights[i].position.xyz - fs_in.fragPos;//计算灯光到片段点的向量，和法线向量点乘出夹角cos值
            float distance = max(length(lightDir), 0.001);
            L = normalize(lightDir);
            attenuation = 1.0 / (lights[i].constant +
                                 lights[i].linear * distance +
                                 lights[i].quadratic * distance * distance);
        }
        else if (lights[i].type == 2)
        {
            // 聚光灯：结合方向和位置，模拟手电筒效果
            vec3 lightDir = normalize(lights[i].position.xyz - fs_in.fragPos); // 片段到光源的方向
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
                float distance = length(lights[i].position.xyz - fs_in.fragPos);
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

        // 计算镜面反射项 Cook-Torrance BRDF
        vec3 numerator = D * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;//0.001避免除0错误
        vec3 specular = numerator / max(denominator, 1e-5);

        // 漫反射项
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS; // 能量守恒，假设入射光全部是由反射光和折射光组成
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        vec3 diffuse = kD * baseColor.rgb / PI;

        // 结合漫反射和镜面反射
        vec3 radiance = lights[i].color.xyz * lights[i].intensity * attenuation * spotEffect;
        Lo += (diffuse + specular) * radiance * NdotL;
    }

    // 环境光（可选）
    float ao = 1.0;//模拟物体表面由于几何结构复杂（如缝隙、凹陷等）而导致的局部阴影效果，需要ao贴图
    vec3 ambient = vec3(0.001) * baseColor.rgb * ao;

    // 最终颜色
    // 计算阴影,目前只应用一个光源的阴影
   // float shadow = ShadowCalculation(fragPos);
    vec3 color = ambient + Lo /**(1 - shadow)*/;
    //色调映射使Lo从LDR的值映射为HDR的值
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
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
