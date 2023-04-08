#ifndef GLOBAL_H
#define GLOBAL_H

#include <glm/glm.hpp>

class PointLight;
class SpotLight;

extern bool directionalLightOn;
extern glm::vec3 directionalLightRot;
extern bool spotLightOn;
extern float cameraSpeed;
extern glm::vec3 modelposition;
extern PointLight pointLightValue;
extern SpotLight spotLightValue;
extern float shininess;

extern bool applyRb;

#endif // GLOBAL_H
