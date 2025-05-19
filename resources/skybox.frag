#version 430 core

in vec3 TexCoords;
out vec4 FragColor;

uniform bool isUseHDRMap;       // 是否有hdr贴图
uniform sampler2D equirectangularMap;//hdr的贴图

vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183); // 1/(2*PI), 1/PI
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = SampleSphericalMap(normalize(TexCoords));
    vec3 color = texture(equirectangularMap, uv).rgb;
    FragColor = vec4(color, 1.0);
}
