#version 330 core
//out vec4 FragColor

uniform vec3 u_LightColor;

void main() {
    gl_FragColor = vec4(u_LightColor, 1.0);
}
