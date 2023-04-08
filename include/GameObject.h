#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "texture.h"
#include "mesh.h"

class DefaultShader;
class Camera;
class Environment;

class GameObject {
    void updateMeshBound(float scale);

protected:
    glm::mat4 trans;
    std::vector<Mesh> meshes;

public:
    GameObject();

    void transToIdentity();
    glm::vec3 getPosition();
    void translate(glm::vec3 t);
    void translate(float x, float y, float z);
    void scale(glm::vec3 s);
    void scale(float x, float y, float z);

    void addTexture(Texture texture); // used by child that only have 1 mesh

    void begin();
    void render(DefaultShader &shader, Camera &perspectiveCamera); // used by object that not reflect light
    void render(DefaultShader &shader, Camera &perspectiveCamera, Environment &environment);
    void end();
    void cleanUp();
};

#endif // GAMEOBJECT_H
