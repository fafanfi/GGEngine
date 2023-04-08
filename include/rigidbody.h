#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>

class RigidBody {
public:
	float mass;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	RigidBody(float mass = 1.0f, glm::vec3 position = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f));
	void applyForce(glm::vec3 force);
    void applyForce(glm::vec3 direction, float magnitude);
    void applyAcceleration(glm::vec3 acc); // for applying acceleration-like gravity, to reduce unnecessaty calculation if the acceleration is constant for all object no matter their mass
    void applyAcceleration(glm::vec3 direction, float magnitude);
    void applyImpulse(glm::vec3 force, float dt);
    void applyImpulse(glm::vec3 direction, float magnitude, float dt);
    void transferEnergy(float joules, glm::vec3 direction);
	void update(float dt);
};

#endif // RIGIDBODY_H
