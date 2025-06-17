#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;


out vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out mat3 outTBN;

uniform mat4 amodel;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;//摄像机位置

void main()
{
    vec3 T = normalize(mat3(amodel) * aTangent);
    vec3 B = normalize(mat3(amodel) * aBitangent);
    vec3 N = normalize(mat3(amodel) * aNormal);
    mat3 TBN = transpose(inverse(mat3(T, B, N)));
    outTBN = TBN;
    vec4 worldPos = amodel * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * worldPos.xyz;

    gl_Position = projection * view * worldPos;
}
