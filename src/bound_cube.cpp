#include "bound_cube.h"
#include "camera.h"
#include "defaultshader.h"

BoundCube::BoundCube() : material(eMaterial::toMMaterial(eMaterial::emerald)) {
    float vertices[] = {
        // position                 normal              texcoord
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,    0.0f, 1.0f
    };

    vao.bind();
    vbo.bind(vertices, sizeof(vertices), GL_STATIC_DRAW);
    vao.setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    vao.setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    vao.setVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    vao.unbind();
    vbo.unbind();
}


void BoundCube::begin() {
    vao.bind();
}

void BoundCube::render(DefaultShader &shader, PerspectiveCamera &perspectiveCamera, glm::mat4 &trans) {
    shader.useProgram();
    shader.setViewPostUniform(perspectiveCamera.Position);
    shader.setMVPMatrixesUniform(trans, perspectiveCamera);

    shader.setMaterialUniforms(material);
    glDrawArrays(GL_LINES, 0, 36);
}

void BoundCube::end() {
    vao.unbind();
}

void BoundCube::cleanUp() {
    vbo.deleteBuffer();
    vao.deleteVertexArray();
}
