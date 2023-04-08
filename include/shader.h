#ifndef SHADER_H
#define SHADER_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <string>

class Shader {
    GLuint vertexShader;
    GLuint fragmentShader;

    std::string loadShaderSource(const char* fileName);

    GLint success;
    static const int infoLogSize = 1024;
    char infoLog[infoLogSize];

    void deleteVertexShader();
    void deleteFragmentShader();

    protected:
        GLuint shaderProgram;

        Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        void deleteShaders();
        void deleteProgram();

    public:
        void useProgram();
        void cleanUp();
};

#endif // SHADER_H
