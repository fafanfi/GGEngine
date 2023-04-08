#include "TextShader.h"

#include <glm/gtc/type_ptr.hpp>

TextShader::TextShader(const char* vertexShaderSource, const char* fragmentShaderSource) : Shader(vertexShaderSource, fragmentShaderSource) {
    uniformProjLocation = glGetUniformLocation(shaderProgram, "u_Proj");
    uniformTextureLocation = glGetUniformLocation(shaderProgram, "u_Texture");
    uniformColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
}

void TextShader::setUniforms(glm::mat4 proj, GLuint slot, glm::vec3 color) {
    glUniformMatrix4fv(uniformProjLocation, 1, false, glm::value_ptr(proj));
    glUniform3fv(uniformColorLocation, 1, glm::value_ptr(color));
    glUniform1i(uniformTextureLocation, slot);
}
