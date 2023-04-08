#ifndef DEFAULT_SHADER_H
#define DEFAULT_SHADER_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shader.h"

class Camera;
class mMaterial;
class PointLight;
class SpotLight;
class DirectionalLight;

typedef struct SpotLightUniformLocationMember {
    GLint uniformSpotLightPositionLocation;
    GLint uniformSpotLightDirectionLocation;
    GLint uniformSpotLightCutOffLocation;
    GLint uniformSpotLightOutterCutOffLocation;
    GLint uniformSpotLightAmbientLocation;
    GLint uniformSpotLightDiffuseLocation;
    GLint uniformSpotLightSpecularLocation;
    GLint uniformSpotLightK0Location;
    GLint uniformSpotLightK1Location;
    GLint uniformSpotLightK2Location;
} SpotLightUniformLocationMember;

typedef struct PointLightUniformLocationMember {
    GLint uniformPointLightPositionLocation;
    GLint uniformPointLightAmbientLocation;
    GLint uniformPointLightDiffuseLocation;
    GLint uniformPointLightSpecularLocation;
    GLint uniformPointLightK0Location;
    GLint uniformPointLightK1Location;
    GLint uniformPointLightK2Location;
} PointLightUniformLocationMember;

#define MAX_NUMBER_OF_POINT_LIGHT 20
#define MAX_NUMBER_OF_SPOT_LIGHT 5

class DefaultShader : public Shader {
    // uniform locations
    GLint uniformTransLocation;
//    GLint uniformModelLocation;
    GLint uniformViewLocation;
    GLint uniformProjLocation;
    GLint uniformMVPLocation;

    GLint uniformNoDiffuseTextureLocation;
    GLint uniformNoSpecularTextureLocation;
    GLint uniformDiffuseTextureLocation;
    GLint uniformSpecularTextureLocation;

    GLint uniformViewPosLocation;

    GLint uniformNumOfLampLocation;
    PointLightUniformLocationMember uniformPointLightsLocationMembers[MAX_NUMBER_OF_POINT_LIGHT];

    GLint uniformDirectionalLightDirectionLocation;
    GLint uniformDirectionalLightAmbientLocation;
    GLint uniformDirectionalLightDiffuseLocation;
    GLint uniformDirectionalLightSpecularLocation;

    GLint uniformNumOfSpotLightLocation;
    SpotLightUniformLocationMember uniformSpotLightsLocationMembers[MAX_NUMBER_OF_SPOT_LIGHT];

//    GLint uniformMaterialAmbientLocation;
    GLint uniformMaterialDiffuseLocation;
    GLint uniformMaterialSpecularLocation;
    GLint uniformMaterialShininessLocation;

    public:
        DefaultShader(const char* vertexShaderSource, const char* fragmentShaderSource);
        void setMVPMatrixesUniform(glm::mat4 &trans, Camera &perspectiveCamera);
        void setViewPostUniform(glm::vec3 &position);
        void setIsSpotLightOnUniform(int on);
        void setNoDiffuseTextureUniform(int n);
        void setNoSpecularTextureUniform(int n);
        void setDiffuseTextureUniform(GLint unit);
        void setSpecularTextureUniform(GLint unit);
        void setMaterialUniforms(mMaterial &material);
        void setNumOfPointLightUniform(int num);
//        void setPointLightUniform(PointLight &lamp);
        void setPointLightsUniforms(PointLight &light, int index);
        void setNumOfSpotLightUniform(int num);
//        void setSpotLightUniform(SpotLight &light);
        void setSpotLightsUniforms(SpotLight &light, int index);
        void setDirectionalLightUniforms(DirectionalLight &light);
};

#endif // DEFAULT_SHADER_H
