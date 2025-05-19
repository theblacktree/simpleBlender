#version 430 core
layout (location = 0)in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBiTangent;

uniform mat4 amodel;
uniform mat4 aview;
uniform mat4 aprojection;
uniform vec3 acameraPos;

out vec3 fragPos;
out vec4 aOutColor;
out vec3 normal;
out vec2 texCoord;
out vec3 viewDir;
out mat3 TBN;//切线空间到世界空间或视图空间的转换矩阵

void main(void)
{
    // 计算世界坐标位置
    vec4 worldPos = amodel * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;

    // 计算法线向量，并转换到世界空间
    mat3 normalMatrix = transpose(inverse(mat3(amodel)));
    normal = normalize(normalMatrix * aNormal);

    //传递纹理坐标
    texCoord = aTexCoord;

    // 转换法线、切线和副切线到世界空间
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBiTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    // 构造TBN矩阵
    TBN = mat3(T, B, N);
    // 计算视线向量
    viewDir = normalize(acameraPos - fragPos);

    gl_Position = aprojection* aview * worldPos;
    aOutColor =vec4(aColor, 1.0);
}
