#ifndef LAMPSHADER_H
#define LAMPSHADER_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "defaultshader.h"

class LampShader : public DefaultShader {
    GLuint uniformLightColorLocation;

    public:
        LampShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        void setLampColorUniform(glm::vec3 &lampColor);
};

#endif // LAMPSHADER_H
