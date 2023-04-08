#version 330 core
//out vec4 FragColor

struct Material {
    vec4 diffuse;
};

uniform Material u_Material;

void main() {
    gl_FragColor = u_Material.diffuse;
//    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
