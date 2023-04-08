#ifndef EBO_H
#define EBO_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <vector>

class EBO {
    GLuint id;

    public:
        EBO();
        void bind(const unsigned int* indices, GLsizeiptr size, GLenum usage);
        void bind(std::vector<unsigned int> &indices, GLenum usage);
        void unbind();
        void deleteBuffer();
};

#endif // EBO_H
