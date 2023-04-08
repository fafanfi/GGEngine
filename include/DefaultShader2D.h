#ifndef DEFAULTSHADER2D_H
#define DEFAULTSHADER2D_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "defaultshader.h"

class OrthographicCamera;

class DefaultShader2D : public DefaultShader {
    GLuint uniformColorLocation;
    GLuint uniformTextureLocation;

public:
    DefaultShader2D(const char* vertexShaderSource, const char* fragmentShaderSource);
    void setColorUniform(glm::vec4 color);
    void setTextureUniform(GLint unit);
};

#endif // DEFAULTSHADER2D_H
