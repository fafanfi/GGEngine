#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>

class VAO {
    GLuint id;

    public:
        VAO();
        void bind();
        void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
        void unbind();
        void deleteVertexArray();
};

#endif // VAO_H
