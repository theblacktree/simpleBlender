#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 amodel;
out vec3 localPos;

void main()
{
    localPos = aPos;

    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = projection * rotView * amodel * vec4(localPos, 1.0);

    gl_Position = clipPos;//.xyww;//深度值为1,永远不会被遮挡,天空盒需要，而小房子的物体加入后会黑屏
}
