#version 330 core
in vec2 v_TexCoords;

uniform sampler2D u_Texture;
uniform vec3 u_Color;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoords).r);
    gl_FragColor = vec4(u_Color, 1.0) * sampled;
}
