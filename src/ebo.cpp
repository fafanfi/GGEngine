#include "ebo.h"

EBO::EBO() {
    glGenBuffers(1, &id);
}

void EBO::bind(const unsigned int* indices, GLsizeiptr size, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
}

void EBO::bind(std::vector<unsigned int> &indices, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], usage);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::deleteBuffer() {
    glDeleteBuffers(1, &id);
}
