#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "material.h"
#include "bounds.h"
#include "vertex.h"
#include "texture.h"
#include "bound_cube.h"
#include "defaultshader.h"

class Camera;
class Environment;

class Mesh {
    VAO vao;
    VBO vbo;
    EBO ebo;

//    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    mMaterial material;
    unsigned int numOfVertices;
    unsigned int numOfIndices;
    bool drawArray;

    void deleteBuffers();

public:
    std::vector<Texture> textures;
    BoundingRegion boundingRegion;

    Mesh(const float* vertices, GLuint verticesDataSize, eMaterial material, BoundingRegion br);
    Mesh(const float* vertices, GLuint verticesDataSize, const unsigned int* indices, GLuint indicesDataSize, eMaterial material, BoundingRegion br);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, mMaterial material, BoundingRegion br);

    bool intersectsWith(BoundingRegion &br, glm::vec3 &distance);
    void begin();
    void render(DefaultShader &shader, Camera &perspectiveCamera, glm::mat4 &trans); // used by object that not reflect light
    void render(DefaultShader &shader, Camera &perspectiveCamera, glm::mat4 &trans, Environment &environment);
    void end();
    void cleanUp();
};

#endif // MESH_H
