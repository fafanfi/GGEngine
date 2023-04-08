#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::bind() {
    // tell opengl which vertex array object is active
    glBindVertexArray(id);
}

// bisa di revisi
void VAO::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
    // set our vertex attributes pointers
    // tell opengl how to interpret this data
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::deleteVertexArray() {
    glDeleteVertexArrays(1, &id);
}
