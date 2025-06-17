#version 430 core
layout (location = 0)in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBiTangent;

uniform mat4 amodel;
uniform mat4 aview;
uniform mat4 aprojection;
uniform vec3 acameraPos;

out vec3 fragPos;//world pos, to calculate light must to use object world pos
out vec4 aOutColor;
out vec3 normal;
out vec2 texCoord;
out vec3 viewDir;
out mat3 TBN;//切线空间到世界空间或视图空间的转换矩阵

void main(void)
{
    // 计算世界坐标位置,由模型矩阵变换到世界坐标系中
    vec4 worldPos = amodel * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;

    // 计算法线向量，并转换到世界空间，使用法线矩阵，使之去除法线位移的功能
    //优化，将法线矩阵在cpu中计算出，然后用uniform变量传到着色器中。
    mat3 normalMatrix = transpose(inverse(mat3(amodel)));//法线矩阵，定义为模型矩阵的逆矩阵的转置矩阵，learnopengl中基础光照介绍
    normal = normalize(normalMatrix * aNormal);//法线向量和灯光向量点乘计算漫反射的贡献

    //传递纹理坐标
    texCoord = aTexCoord;

    // 转换法线、切线和副切线到世界空间
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBiTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    // 构造TBN矩阵
    TBN = mat3(T, B, N);
    // 计算视线向量
    //这个是观察者向量，它和灯光在物体表面反射后的向量点乘计算出高光部分的贡献，夹角越小贡献越高。
    viewDir = normalize(acameraPos - fragPos);

    gl_Position = aprojection * aview * worldPos;
    aOutColor = aColor;
}
