#ifndef LIGHT_H
#define LIGHT_H

#include "cube.h"

class LampShader;
class PerspectiveCamera;

class Light {
protected:
    Light(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float k0, float k1, float k2);

public:
    // value strength
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    float k0;
    float k1;
    float k2;
};

class PointLight : public Light {
public:
    glm::vec3 position;

    PointLight(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float k0, float k1, float k2);
};

class DirectionalLight : public Light {
public:
    glm::vec3 direction;

    DirectionalLight(glm::vec3 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
//    void render(DefaultShader &shader);
};

class SpotLight : public Light {
public:
    glm::vec3 position;
    glm::vec3 direction;

    float cutOff;
    float outterCutOff;

    SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outterCutOff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float k0, float k1, float k2);
};

class Lamp : public Cube {
public:
    glm::vec3 lampColor;

    Lamp(glm::vec3 lampColor);
    void render(LampShader &shader, PerspectiveCamera &perspectiveCamera);
};

#endif // LAMP_H
