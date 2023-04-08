#include "vbo.h"

VBO::VBO() {
    glGenBuffers(1, &id);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

// copy our vertices array in a buffer for OpenGL to use
void VBO::bind(const float* vertices, GLsizeiptr size, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

void VBO::bind(std::vector<Vertex> &vertices, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], usage);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::deleteBuffer() {
    glDeleteBuffers(1, &id);
}
