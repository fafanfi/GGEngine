#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <glm/glm.hpp>

#include <vector>

#include "light.h"

class LampShader;
class PerspectiveCamera;

class Environment {
public:
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;
    std::vector<Lamp> lamps;
    std::vector<SpotLight> spotLights;

    static const glm::vec3 gravitationalAcceleration;

    void render(LampShader &shader, PerspectiveCamera &perspectiveCamera);
    void addLight(DirectionalLight directionalLight);
    void addLight(PointLight pointLight);
    void addLight(SpotLight spotLight);
    void addLamp(Lamp lamp);
};

#endif // ENVIRONMENT_H
