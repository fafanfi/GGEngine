#include "mesh.h"
#include "global.h"
#include "defaultshader.h"
#include "texture.h"
#include "environment.h"
#include "light.h"
#include "bound_cube.h"
#include "material.h"
#include "camera.h"

#include <glm/gtx/transform.hpp>

Mesh::Mesh(const float* vertices, GLuint verticesDataSize, eMaterial material, BoundingRegion br) : indices({}), material(eMaterial::toMMaterial(material)), textures({}), boundingRegion(br) {
    // tell opengl which vertex array object is active
    vao.bind();
    // copy our vertices array in a buffer for OpenGL to use
    vbo.bind(vertices, verticesDataSize, GL_STATIC_DRAW);
    // set our vertex attributes pointers
    // tell opengl how to interpret this data
    vao.setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    vao.setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    vao.setVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    // experimenting
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    numOfVertices = (verticesDataSize / sizeof(float)) / 8;
    drawArray = true;
}

Mesh::Mesh(const float* vertices, GLuint verticesDataSize, const unsigned int* indices, GLuint indicesDataSize, eMaterial material, BoundingRegion br) : material(eMaterial::toMMaterial(material)), textures({}), boundingRegion(br) {
    // tell opengl which vertex array object is active
    vao.bind();
    // copy our vertices array in a buffer for OpenGL to use
    vbo.bind(vertices, verticesDataSize, GL_STATIC_DRAW);
    // set our vertex attributes pointers
    // tell opengl how to interpret this data
    vao.setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
//    vao.setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
//    vao.setVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    ebo.bind(indices, indicesDataSize, GL_STATIC_DRAW);

    // experimenting
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

//    numberOfVertices = (verticesDataSize / sizeof(float)) / 8;
    numOfIndices = indicesDataSize / sizeof(unsigned int);
    drawArray = false;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, mMaterial material, BoundingRegion br) : indices(indices), textures(textures), material(material), boundingRegion(br) {
    vao.bind();
    vbo.bind(vertices, GL_STATIC_DRAW);
    vao.setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
    vao.setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    vao.setVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, textCoord));
    ebo.bind(indices, GL_STATIC_DRAW);

    // experimenting
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

//    numberOfVertices = vertices.size();
    numOfIndices = indices.size();
    drawArray = false;
}

bool Mesh::intersectsWith(BoundingRegion &br, glm::vec3 &distance) {
    return boundingRegion.intersectsWith(br, distance);
}

void Mesh::begin() {
    vao.bind();
}

// used by object that not reflect light
void Mesh::render(DefaultShader &shader, Camera &perspectiveCamera, glm::mat4 &trans) {
    shader.useProgram();
    shader.setViewPostUniform(perspectiveCamera.Position);
    shader.setMVPMatrixesUniform(trans, perspectiveCamera);

    if (textures.empty()) {
        shader.setNoDiffuseTextureUniform(1);
        shader.setNoSpecularTextureUniform(1);
    } else {
        bool noDiffuseTexture = true;
        bool noSpecularTexture = true;

        for (Texture &texture : textures) {
            texture.activate();
            texture.bind();

            if (texture.type == aiTextureType_DIFFUSE) {
                shader.setDiffuseTextureUniform(0);
                shader.setNoDiffuseTextureUniform(0);
                noDiffuseTexture = false;
            } else if (texture.type == aiTextureType_SPECULAR) {
                shader.setSpecularTextureUniform(1);
                shader.setNoSpecularTextureUniform(0);
                noSpecularTexture = false;
            }

            if (noDiffuseTexture) {
                shader.setNoDiffuseTextureUniform(1);
            }

            if (noSpecularTexture) {
                shader.setNoSpecularTextureUniform(1);
            }
        }
    }

    shader.setMaterialUniforms(material);

    if (drawArray) {
        glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
    } else {
        glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
    }
}

DirectionalLight dl0(glm::vec3(0.0f), glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(0.0f));

void Mesh::render(DefaultShader &shader, Camera &perspectiveCamera, glm::mat4 &trans, Environment &environment) {
    shader.useProgram();

    if (!environment.directionalLights.empty()) {
        if (directionalLightOn) {
            shader.setDirectionalLightUniforms(environment.directionalLights[0]);
        } else {
            shader.setDirectionalLightUniforms(dl0);
        }
    }

    if (spotLightOn) {
        shader.setNumOfSpotLightUniform(environment.spotLights.size());

        for (int i = 0; i < environment.spotLights.size(); i++) {
            shader.setSpotLightsUniforms(environment.spotLights[i], i);
        }
    } else {
        shader.setNumOfSpotLightUniform(0);
    }

    shader.setNumOfPointLightUniform(environment.pointLights.size());

    for (int i = 0; i < environment.pointLights.size(); i++) {
        shader.setPointLightsUniforms(environment.pointLights[i], i);
    }

    render(shader, perspectiveCamera, trans);
}

void Mesh::end() {
    vao.unbind();

    for (Texture &texture : textures) {
        texture.unbind();
    }

    glActiveTexture(0);
}

void Mesh::deleteBuffers() {
    vao.deleteVertexArray();
    vbo.deleteBuffer();
    ebo.deleteBuffer();
}

void Mesh::cleanUp() {
    deleteBuffers();

    for (Texture &texture : textures) {
        texture.deleteTexture();
    }
}
