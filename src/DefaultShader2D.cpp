#include "DefaultShader2D.h"

#include <glm/gtc/type_ptr.hpp>

DefaultShader2D::DefaultShader2D(const char* vertexShaderSource, const char* fragmentShaderSource) : DefaultShader(vertexShaderSource, fragmentShaderSource) {
    uniformColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
    uniformTextureLocation = glGetUniformLocation(shaderProgram, "u_Texture");
}

void DefaultShader2D::setColorUniform(glm::vec4 color) {
    glUniform4fv(uniformColorLocation, 1, glm::value_ptr(color));
}

void DefaultShader2D::setTextureUniform(GLint unit) {
    glUniform1i(uniformTextureLocation, unit);
}
