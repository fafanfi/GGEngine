#version 330 core
//out vec4 FragColor

struct Material {
//    vec3 ambient;
    vec4 diffuse;
    vec4 specular;
//    sampler2D diffuse;
//    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float k0;
    float k1;
    float k2;
};

struct DirectionalLight {
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outterCutOff;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float k0;
    float k1;
    float k2;
};

uniform int u_NoDiffuseTexture;
uniform int u_NoSpecularTexture;
uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;

#define MAX_NUMBER_OF_POINT_LIGHT 20
#define MAX_NUMBER_OF_SPOT_LIGHT 5

uniform int u_NumOfPointLight;
uniform int u_NumOfSpotLight;

uniform Material u_Material;
uniform PointLight u_PointLights[MAX_NUMBER_OF_POINT_LIGHT];
uniform PointLight u_PointLight;
uniform DirectionalLight u_DirectionalLight;
uniform SpotLight u_SpotLights[MAX_NUMBER_OF_SPOT_LIGHT];
uniform SpotLight u_SpotLight;

uniform vec3 u_ViewPos;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;

vec4 calcDirectionalLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {
    // ambient
    vec4 ambient = u_DirectionalLight.ambient * diffMap;
    // diffuse
    vec3 lightDir = normalize(-u_DirectionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = u_DirectionalLight.diffuse * (diff * diffMap);
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess * 128);
    vec4 specular = u_DirectionalLight.specular * (spec * specMap);

    return (ambient + diffuse + specular);
}

vec4 calcPointLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {
    // ambient
//    vec3 ambient = u_PointLight.ambient * u_Material.ambient;
//    vec3 ambient = u_PointLight.ambient * vec3(texture2D(u_Texture1, v_TexCoord));
    vec4 ambient = u_PointLights[idx].ambient * diffMap;

    // diffuse
    vec3 lightDir = normalize(u_PointLights[idx].position - v_Position);
    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = u_PointLight.diffuse * (diff * u_Material.diffuse);
//    vec3 diffuse = u_PointLight.diffuse * (diff * vec3(texture2D(u_Texture1, v_TexCoord)));
    vec4 diffuse = u_PointLights[idx].diffuse * (diff * diffMap);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess * 128);
//    vec3 specular = u_PointLight.specular * (spec * u_Material.specular);
//    vec3 specular = u_PointLight.specular * (spec * vec3(texture2D(u_Texture2, v_TexCoord)));
    vec4 specular = u_PointLights[idx].specular * (spec * specMap);

    float dist = length(u_PointLights[idx].position - v_Position);
    float attenuation = 1.0 / (u_PointLights[idx].k0 + u_PointLights[idx].k1 * dist + u_PointLights[idx].k2 * (dist * dist));

    return (ambient + diffuse + specular) * attenuation;
}

vec4 calcSpotLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {
    vec3 lightDir = normalize(u_SpotLights[idx].position - v_Position);
    float theta = dot(lightDir, normalize(-u_SpotLights[idx].direction));

    // ambient
    vec4 ambient = u_SpotLights[idx].ambient * diffMap;

    // > because using cosines and not degrees
    if (theta > u_SpotLights[idx].outterCutOff) {
      // if in cutoff, render light

      //diffuse
      float diff = max(dot(norm, lightDir), 0.0);
      vec4 diffuse = u_SpotLights[idx].diffuse * (diff * diffMap);

      // specular
      vec3 reflectDir = reflect(-lightDir, norm);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess * 128);
      vec4 specular = u_SpotLights[idx].specular * (spec * specMap);

      float intensity = (theta - u_SpotLights[idx].outterCutOff) / (u_SpotLights[idx].cutOff - u_SpotLights[idx].outterCutOff);
      intensity = clamp(intensity, 0.0, 1.0);
      diffuse *= intensity;
      specular *= intensity;

      float dist = length(u_SpotLights[idx].position - v_Position);
      float attenuation = 1.0 / (u_SpotLights[idx].k0 + u_SpotLights[idx].k1 * dist + u_SpotLights[idx].k2 * (dist * dist));

      return (ambient + diffuse + specular) * attenuation;
    } else {
      // render just ambient
      return ambient;
    }
}

void main() {
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_Position);
    vec4 diffMap;
    vec4 specMap;

    if (u_NoDiffuseTexture == 1 && u_NoSpecularTexture == 1) {
        diffMap = u_Material.diffuse;
        specMap = u_Material.specular;
    } else if (u_NoDiffuseTexture == 1) {
        diffMap = u_Material.diffuse;
        specMap = texture2D(u_SpecularTexture, v_TexCoord);
    } else if (u_NoSpecularTexture == 1) {
        diffMap = texture2D(u_DiffuseTexture, v_TexCoord);
        specMap = u_Material.specular;
    } else {
        diffMap = texture2D(u_DiffuseTexture, v_TexCoord);
        specMap = texture2D(u_SpecularTexture, v_TexCoord);
    }

    vec4 result = calcDirectionalLight(norm, viewDir, diffMap, specMap);

    for (int i = 0; i < u_NumOfPointLight; i++) {
        result += calcPointLight(i, norm, viewDir, diffMap, specMap);
    }

    for (int i = 0; i < u_NumOfSpotLight; i++) {
        result += calcSpotLight(i, norm, viewDir, diffMap, specMap);
    }

    // depth testing
    float near = 0.01;
    float far = 2000.0;
    // convert depth coordinate
    float z = gl_FragCoord.z * 2.0 - 1.0; // transform to NDC [0, 1] => [-1, 1]
    float linearDepth = (2.0 * near * far) / (z * (far - near) - (far + near)); // take inverse of the projection matrix (perspective)
    float factor = (near + linearDepth) / (near - far); // convert back to [0, 1]

    result.rgb *= 1 - factor;

    gl_FragColor = result;
}
