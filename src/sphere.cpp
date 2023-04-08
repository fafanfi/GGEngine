#include "sphere.h"
#include "camera.h"

#include <glm/gtx/transform.hpp>

Sphere::Sphere() : Model("assets/models/sphere.obj", BoundTypes::SPHERE) {

}

void Sphere::render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, Environment &environment, float dt) {
    rigidBody.update(dt);
    transToIdentity();
    translate(rigidBody.position);

    GameObject::render(shader, perspectiveCamera, environment);
}

// used for modelarray template class
void Sphere::render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, Environment &environment) {
    GameObject::render(shader, perspectiveCamera, environment);
}
