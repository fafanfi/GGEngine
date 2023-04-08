#include "model_array.h"

#include <glm/gtx/transform.hpp>

SphereArray::SphereArray() {
    model = Sphere();
}

void SphereArray::render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, Environment &environment, float dt) {
    for (RigidBody &rb : rbs) {
        rb.update(dt);
        model.transToIdentity();
        model.translate(rb.position);
        model.render(shader, perspectiveCamera, environment);
    }
}
