#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_Trans;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 v_Position;
out vec2 v_TexCoord;

void main() {
    vec4 tPos = u_Trans * vec4(a_Position, 1.0f);
    v_Position = tPos.xyz;
    v_TexCoord = a_TexCoord;

    gl_Position = u_Proj * u_View * tPos;
}
