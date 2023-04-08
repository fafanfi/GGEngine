#include "GameObject.h"
#include "mesh.h"

#include <glm/gtx/transform.hpp>

GameObject::GameObject() : trans(glm::mat4(1.0f)) {
    //ctor
}

void GameObject::transToIdentity() {
    trans = glm::mat4(1.0f);
}

glm::vec3 GameObject::getPosition() {
    return trans * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void GameObject::translate(glm::vec3 t) {
    trans = glm::translate(trans, t);
    updateMeshBound(1.0f);
}

void GameObject::translate(float x, float y, float z) {
    trans = glm::translate(trans, glm::vec3(x, y, z));
    updateMeshBound(1.0f);
}

void GameObject::scale(glm::vec3 s) {
    trans = glm::scale(trans, s);
    updateMeshBound(s.x); // ATTENTION
}

void GameObject::scale(float x, float y, float z) {
    trans = glm::scale(trans, glm::vec3(x, y, z));
    updateMeshBound(x); // ATTENTION
}

void GameObject::updateMeshBound(float scale) {
    glm::vec3 pos = getPosition();

    for (Mesh &mesh : meshes) {
        mesh.boundingRegion.update(pos, scale);
    }
}

// used by object that only have 1 mesh
void GameObject::addTexture(Texture texture) {
    meshes[0].textures.push_back(texture);
}

void GameObject::begin() {
//    for (Mesh mesh : meshes) {
//        mesh.begin();
//    }
}

// used by object that not reflect light
void GameObject::render(DefaultShader &shader, Camera &perspectiveCamera) {
    for (Mesh &mesh : meshes) {
        mesh.begin();
        mesh.render(shader, perspectiveCamera, trans);
    }
}

void GameObject::render(DefaultShader &shader, Camera &perspectiveCamera, Environment &environment) {
    for (Mesh &mesh : meshes) {
        mesh.begin();
        mesh.render(shader, perspectiveCamera, trans, environment);
    }
}

void GameObject::end() {
    for (Mesh &mesh : meshes) {
        mesh.end();
    }
}

void GameObject::cleanUp() {
    for (Mesh &mesh : meshes) {
        mesh.cleanUp();
    }
}
