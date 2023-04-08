#version 330 core

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec3 v_Position;
//in vec2 v_TexCoord;

void main() {
    gl_FragColor = u_Color;
}
