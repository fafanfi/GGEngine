#ifndef SPHERE_H
#define SPHERE_H

#include "model.h"
#include "rigidbody.h"

class Sphere : public Model {
    public:
        RigidBody rigidBody;

        Sphere();
        void render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, Environment &environment, float dt);
        void render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, Environment &environment); // used for modelarray template class
};

#endif // SPHERE_H
