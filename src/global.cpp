#include "global.h"
#include "light.h"

bool directionalLightOn = true;
bool spotLightOn = false;
glm::vec3 directionalLightRot;
float cameraSpeed = 0.005f;
glm::vec3 modelposition;
PointLight pointLightValue(glm::vec3(0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 1.0f, 0.5f, 0.5f);
SpotLight spotLightValue(glm::vec3(0.f), glm::vec3(0.f), glm::cos(glm::radians(15.5f)), glm::cos(glm::radians(20.f)), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, .005f, .0052f);
float shininess = 0.5f;

bool applyRb = true;
