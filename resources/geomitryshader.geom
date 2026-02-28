#version 430 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3 * 6) out;

out vec4 WorldPos;

uniform mat4 shadowMatrices[6];

void main()
{
    for (int face = 0; face < 6; face++)
    {
        gl_Layer = face; // 设置当前输出的图层（即立方体贴图的一个面）

        for (int i = 0; i < 3; i++)
        {
            WorldPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * WorldPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
