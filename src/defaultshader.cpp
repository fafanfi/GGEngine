#include "defaultshader.h"
#include "light.h"
#include "camera.h"

#include <glm/gtc/type_ptr.hpp>

DefaultShader::DefaultShader(const char* vertexShaderSource, const char* fragmentShaderSource) : Shader(vertexShaderSource, fragmentShaderSource) {
    uniformTransLocation = glGetUniformLocation(shaderProgram, "u_Trans");
//    uniformModelLocation = glGetUniformLocation(shaderProgram, "u_Model");
    uniformViewLocation = glGetUniformLocation(shaderProgram, "u_View");
    uniformProjLocation = glGetUniformLocation(shaderProgram, "u_Proj");
    uniformMVPLocation = glGetUniformLocation(shaderProgram, "u_MVP");

    uniformNoDiffuseTextureLocation = glGetUniformLocation(shaderProgram, "u_NoDiffuseTexture");
    uniformNoSpecularTextureLocation = glGetUniformLocation(shaderProgram, "u_NoSpecularTexture");
    uniformDiffuseTextureLocation = glGetUniformLocation(shaderProgram, "u_DiffuseTexture");
    uniformSpecularTextureLocation = glGetUniformLocation(shaderProgram, "u_SpecularTexture");

    uniformViewPosLocation = glGetUniformLocation(shaderProgram, "u_ViewPos");
    uniformNumOfSpotLightLocation = glGetUniformLocation(shaderProgram, "u_NumOfSpotLight");

    for (int i = 0; i < MAX_NUMBER_OF_SPOT_LIGHT; i++) {
        std::string uniformName = "u_SpotLights[" + std::to_string(i) + "]";

        uniformSpotLightsLocationMembers[i].uniformSpotLightPositionLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".position").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightDirectionLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".direction").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightCutOffLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".cutOff").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightOutterCutOffLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".outterCutOff").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightAmbientLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".ambient").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightDiffuseLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".diffuse").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightSpecularLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".specular").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightK0Location = glGetUniformLocation(shaderProgram, std::string(uniformName + ".k0").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightK1Location = glGetUniformLocation(shaderProgram, std::string(uniformName + ".k1").c_str());
        uniformSpotLightsLocationMembers[i].uniformSpotLightK2Location = glGetUniformLocation(shaderProgram, std::string(uniformName + ".k2").c_str());
    }

    uniformNumOfLampLocation = glGetUniformLocation(shaderProgram, "u_NumOfPointLight");

    for (int i = 0; i < MAX_NUMBER_OF_POINT_LIGHT; i++) {
        std::string uniformName = "u_PointLights[" + std::to_string(i) + "]";

        uniformPointLightsLocationMembers[i].uniformPointLightPositionLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".position").c_str());
        uniformPointLightsLocationMembers[i].uniformPointLightAmbientLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".ambient").c_str());
        uniformPointLightsLocationMembers[i].uniformPointLightDiffuseLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".diffuse").c_str());
        uniformPointLightsLocationMembers[i].uniformPointLightSpecularLocation = glGetUniformLocation(shaderProgram, std::string(uniformName + ".specular").c_str());
        uniformPointLightsLocationMembers[i].uniformPointLightK0Location = glGetUniformLocation(shaderProgram, std::string(uniformName + ".k0").c_str());
        uniformPointLightsLocationMembers[i].uniformPointLightK1Location = glGetUniformLocation(shaderProgram, std::string(uniformName + ".k1").c_str());
        uniformPointLightsLocationMembers[i].uniformPointLightK2Location = glGetUniformLocation(shaderProgram, std::string(uniformName + ".k2").c_str());
    }

    uniformDirectionalLightDirectionLocation = glGetUniformLocation(shaderProgram, "u_DirectionalLight.direction");
    uniformDirectionalLightAmbientLocation = glGetUniformLocation(shaderProgram, "u_DirectionalLight.ambient");
    uniformDirectionalLightDiffuseLocation = glGetUniformLocation(shaderProgram, "u_DirectionalLight.diffuse");
    uniformDirectionalLightSpecularLocation = glGetUniformLocation(shaderProgram, "u_DirectionalLight.specular");

//    uniformMaterialAmbientLocation = glGetUniformLocation(shaderProgram, "u_Material.ambient");
    uniformMaterialDiffuseLocation = glGetUniformLocation(shaderProgram, "u_Material.diffuse");
    uniformMaterialSpecularLocation = glGetUniformLocation(shaderProgram, "u_Material.specular");
    uniformMaterialShininessLocation = glGetUniformLocation(shaderProgram, "u_Material.shininess");

    deleteShaders();
}

void DefaultShader::setMVPMatrixesUniform(glm::mat4 &trans, Camera &perspectiveCamera) {

//    glUniformMatrix4fv(uniformModelLocation, 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(uniformTransLocation, 1, false, glm::value_ptr(trans));
    glUniformMatrix4fv(uniformViewLocation, 1, false, glm::value_ptr(perspectiveCamera.View));
    glUniformMatrix4fv(uniformProjLocation, 1, false, glm::value_ptr(perspectiveCamera.Proj));
    glUniformMatrix4fv(uniformMVPLocation, 1, false, glm::value_ptr(perspectiveCamera.Proj * perspectiveCamera.View * trans));
}

void DefaultShader::setNoDiffuseTextureUniform(int n) {
        glUniform1i(uniformNoDiffuseTextureLocation, n);
}

void DefaultShader::setNoSpecularTextureUniform(int n) {
        glUniform1i(uniformNoSpecularTextureLocation, n);
}

void DefaultShader::setDiffuseTextureUniform(GLint unit) {
        glUniform1i(uniformDiffuseTextureLocation, unit);
}

void DefaultShader::setSpecularTextureUniform(GLint unit) {
        glUniform1i(uniformSpecularTextureLocation, unit);
}

void DefaultShader::setMaterialUniforms(mMaterial &material) {
//    glUniform4fv(uniformMaterialAmbientLocation, 1, glm::value_ptr(material.ambient));
    glUniform4fv(uniformMaterialDiffuseLocation, 1, glm::value_ptr(material.diffuse));
    glUniform4fv(uniformMaterialSpecularLocation, 1, glm::value_ptr(material.specular));
    glUniform1f(uniformMaterialShininessLocation, material.shininess);
}

void DefaultShader::setViewPostUniform(glm::vec3 &position) {
    glUniform3fv(uniformViewPosLocation, 1, glm::value_ptr(position));
}

void DefaultShader::setNumOfPointLightUniform(int num) {
    glUniform1i(uniformNumOfLampLocation, num);
}

void DefaultShader::setPointLightsUniforms(PointLight &light, int index) {
    glUniform3fv(uniformPointLightsLocationMembers[index].uniformPointLightPositionLocation, 1, glm::value_ptr(light.position));
    glUniform4fv(uniformPointLightsLocationMembers[index].uniformPointLightAmbientLocation, 1, glm::value_ptr(light.ambient));
    glUniform4fv(uniformPointLightsLocationMembers[index].uniformPointLightDiffuseLocation, 1, glm::value_ptr(light.diffuse));
    glUniform4fv(uniformPointLightsLocationMembers[index].uniformPointLightSpecularLocation, 1, glm::value_ptr(light.specular));
    glUniform1f(uniformPointLightsLocationMembers[index].uniformPointLightK0Location, light.k0);
    glUniform1f(uniformPointLightsLocationMembers[index].uniformPointLightK1Location, light.k1);
    glUniform1f(uniformPointLightsLocationMembers[index].uniformPointLightK2Location, light.k2);
}

void DefaultShader::setNumOfSpotLightUniform(int num) {
    glUniform1i(uniformNumOfSpotLightLocation, num);
}

void DefaultShader::setSpotLightsUniforms(SpotLight &light, int index) {
    glUniform3fv(uniformSpotLightsLocationMembers[index].uniformSpotLightPositionLocation, 1, glm::value_ptr(light.position));
    glUniform3fv(uniformSpotLightsLocationMembers[index].uniformSpotLightDirectionLocation, 1, glm::value_ptr(light.direction));
    glUniform1f(uniformSpotLightsLocationMembers[index].uniformSpotLightCutOffLocation, light.cutOff);
    glUniform1f(uniformSpotLightsLocationMembers[index].uniformSpotLightOutterCutOffLocation, light.outterCutOff);
    glUniform4fv(uniformSpotLightsLocationMembers[index].uniformSpotLightAmbientLocation, 1, glm::value_ptr(light.ambient));
    glUniform4fv(uniformSpotLightsLocationMembers[index].uniformSpotLightDiffuseLocation, 1, glm::value_ptr(light.diffuse));
    glUniform4fv(uniformSpotLightsLocationMembers[index].uniformSpotLightSpecularLocation, 1, glm::value_ptr(light.specular));
    glUniform1f(uniformSpotLightsLocationMembers[index].uniformSpotLightK0Location, light.k0);
    glUniform1f(uniformSpotLightsLocationMembers[index].uniformSpotLightK1Location, light.k1);
    glUniform1f(uniformSpotLightsLocationMembers[index].uniformSpotLightK2Location, light.k2);
}

void DefaultShader::setDirectionalLightUniforms(DirectionalLight &light) {
    glUniform3fv(uniformDirectionalLightDirectionLocation, 1, glm::value_ptr(light.direction));
    glUniform4fv(uniformDirectionalLightAmbientLocation, 1, glm::value_ptr(light.ambient));
    glUniform4fv(uniformDirectionalLightDiffuseLocation, 1, glm::value_ptr(light.diffuse));
    glUniform4fv(uniformDirectionalLightSpecularLocation, 1, glm::value_ptr(light.specular));
}
