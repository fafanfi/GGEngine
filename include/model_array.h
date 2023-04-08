#ifndef MODELARRAY_H
#define MODELARRAY_H

#include <vector>

#include "sphere.h"
#include "rigidbody.h"

class DefaultShader;
class PerspectiveCamera;
class Environment;

template <class T>
class ModelArray {
protected:
    std::vector<RigidBody> rbs;

public:
    void cleanUp() {
        model.cleanUp();
    }

    void addInstance(RigidBody rb) {
        rbs.push_back(rb);
    }

protected:
    T model;
};

class SphereArray : public ModelArray<Sphere> {
public:
    SphereArray();

    void render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, Environment &environment, float dt);
};

#endif // MODELARRAY_H
