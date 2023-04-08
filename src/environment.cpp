#include "environment.h"
#include "global.h"
#include "light.h"

const glm::vec3 Environment::gravitationalAcceleration = glm::vec3(0.0f, -9.81f, 0.0f);

void Environment::render(LampShader &shader, PerspectiveCamera &perspectiveCamera) {
    for (Lamp &lamp : lamps) {
        lamp.begin();
        lamp.render(shader, perspectiveCamera);
        lamp.end();
    }
}

void Environment::addLight(DirectionalLight directionalLight) {
    directionalLights.push_back(directionalLight);
}

void Environment::addLight(PointLight pointLight) {
    pointLights.push_back(pointLight);
}

void Environment::addLight(SpotLight spotLight) {
    spotLights.push_back(spotLight);
}

void Environment::addLamp(Lamp lamp) {
    lamps.push_back(lamp);
}
