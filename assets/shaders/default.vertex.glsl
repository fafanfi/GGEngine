#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_Trans;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main() {
    vec4 temptPosition = u_Trans * vec4(a_Position, 1.0f);
    v_Position = temptPosition.xyz;
    v_Normal = mat3(transpose(inverse(u_Trans))) * a_Normal; // update normal vertex direction
    v_TexCoord = a_TexCoord;

    gl_Position = u_Proj * u_View * temptPosition;
}