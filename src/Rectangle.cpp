#include "Rectangle.h"
#include "bounds.h"
#include "mesh.h"
#include "DefaultShader2D.h"
#include "camera.h"

Rectangle::Rectangle(int width, int height, eMaterial material) {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    meshes.push_back(Mesh(vertices, sizeof(vertices), indices, sizeof(indices), material, BoundingRegion(glm::vec3(-0.5f), glm::vec3(0.5f))));
}

Rectangle::Rectangle(int width, int height, glm::vec4 color) : color(color) {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    meshes.push_back(Mesh(vertices, sizeof(vertices), indices, sizeof(indices), eMaterial::black_plastic, BoundingRegion(glm::vec3(-0.5f), glm::vec3(0.5f))));
}

Rectangle::Rectangle(Texture texture) {

}

void Rectangle::render(DefaultShader2D &shader, OrthographicCamera &camera) {
    shader.useProgram();
    shader.setColorUniform(color);
    shader.setMVPMatrixesUniform(trans, camera);
    GameObject::render(shader, camera);
}
