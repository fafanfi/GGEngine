#ifndef TEXTSHADER_H
#define TEXTSHADER_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shader.h"

class TextShader : public Shader {
    GLint uniformProjLocation;
    GLint uniformTextureLocation;
    GLint uniformColorLocation;

public:
    TextShader(const char* vertexShaderSource, const char* fragmentShaderSource);
    void setUniforms(glm::mat4 proj, GLuint slot, glm::vec3 color);
};

#endif // TEXTSHADER_H
