#include "rigidbody.h"

RigidBody::RigidBody(float mass, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration) : mass(mass), position(position), velocity(velocity), acceleration(acceleration) {}

void RigidBody::applyForce(glm::vec3 force) {
    acceleration += force / mass;
}

void RigidBody::applyForce(glm::vec3 direction, float magnitude) {
    applyForce(direction * magnitude);
}

// for applying acceleration-like gravity, to reduce unnecessaty calculation if the acceleration is constant for all object no matter their mass
void RigidBody::applyAcceleration(glm::vec3 acc) {
    acceleration += acc;
}

void RigidBody::applyAcceleration(glm::vec3 direction, float magnitude) {
    applyAcceleration(direction * magnitude);
}

void RigidBody::applyImpulse(glm::vec3 force, float dt) {
    velocity += force / mass * dt;
}

void RigidBody::applyImpulse(glm::vec3 direction, float magnitude, float dt) {
    applyImpulse(direction * magnitude, dt);
}

void RigidBody::transferEnergy(float joules, glm::vec3 direction) {
    if (joules == 0) {
        return;
    }

    // comes from formula: KE = 1/2 * m * v^2
    glm::vec3 deltaV = (float) sqrt(2 * abs(joules) / mass) * direction;

    velocity += joules > 0 ? deltaV : -deltaV;
}

void RigidBody::update(float dt) {
    position += velocity * dt + 0.5f * acceleration * (dt * dt);
    velocity += acceleration * dt;
}
