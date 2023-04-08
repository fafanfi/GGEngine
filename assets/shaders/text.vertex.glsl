#version 330 core
layout (location = 0) in vec4 a_Pos; // <vec2 pos, vec2 tex>

uniform mat4 u_Proj; // projection matrix

out vec2 v_TexCoords;

void main() {
    gl_Position = u_Proj * vec4(a_Pos.xy, 0.0, 1.0);
    v_TexCoords = a_Pos.zw;
}

