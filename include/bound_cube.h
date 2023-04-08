#ifndef BOUNDCUBE_H
#define BOUNDCUBE_H

#include "vao.h"
#include "vbo.h"
#include "material.h"

class DefaultShader;
class PerspectiveCamera;

class BoundCube {
    VAO vao;
    VBO vbo;
    mMaterial material;

public:
    BoundCube();
    void begin();
    void render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, glm::mat4 &trans);
    void end();
    void cleanUp();
};

#endif // BOUNDCUBE_H
