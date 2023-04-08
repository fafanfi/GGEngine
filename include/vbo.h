#ifndef VBO_H
#define VBO_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <vector>

#include "vertex.h"

class VBO {
    GLuint id;

    public:
        VBO();
        void bind();
        void bind(const float* vertices, GLsizeiptr size, GLenum usage);
        void bind(std::vector<Vertex> &vertices, GLenum usage);
        void unbind();
        void deleteBuffer();
};

#endif // VBO_H
