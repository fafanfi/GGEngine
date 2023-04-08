#include "light.h"
#include "lamp_shader.h"
#include "camera.h"

///////// LIGHT

Light::Light(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float k0, float k1, float k2) : ambient(ambient), diffuse(diffuse), specular(specular), k0(k0), k1(k1), k2(k2) {}

///////// DIRECTIONAL LIGHT

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular) : Light(ambient, diffuse, specular, 0.0f, 0.0f, 0.0f), direction(direction) {
}

//void DirectionalLight::render(DefaultShader &shader) {
//    shader.useProgram();
//    shader.setDirectionalLightUniform(*this);
//}

///////// POINT LIGHT

PointLight::PointLight(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float k0, float k1, float k2) : Light(ambient, diffuse, specular, k0, k1, k2), position(position) {
}

///////// SPOT LIGHT

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outterCutOff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float k0, float k1, float k2) : Light(ambient, diffuse, specular, k0, k1, k2), position(position), direction(direction), cutOff(cutOff), outterCutOff(outterCutOff) {
}

//////// LAMP
Lamp::Lamp(glm::vec3 lampColor) : Cube(eMaterial::white_plastic), lampColor(lampColor) {}

void Lamp::render(LampShader &shader, PerspectiveCamera &perspectiveCamera) {
    shader.useProgram();
    shader.setLampColorUniform(lampColor);

    GameObject::render(shader, perspectiveCamera);
}
