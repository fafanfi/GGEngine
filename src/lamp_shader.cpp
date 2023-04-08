#include "lamp_shader.h"

#include <glm/gtc/type_ptr.hpp>

LampShader::LampShader(const char* vertexShaderSource, const char* fragmentShaderSource) : DefaultShader(vertexShaderSource, fragmentShaderSource) {
    uniformLightColorLocation = glGetUniformLocation(shaderProgram, "u_LightColor");

    deleteShaders();
}

void LampShader::setLampColorUniform(glm::vec3 &lampColor) {
    glUniform3fv(uniformLightColorLocation, 1, glm::value_ptr(lampColor));
}
