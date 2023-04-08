#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <assimp/scene.h>

/*
    material structure to contain lighting values for different materials
*/

typedef struct mMaterial {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
} mMaterial;

typedef struct eMaterial {
    // lighting values
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    /*
        static instances of common materials
    */

    static eMaterial emerald;
    static eMaterial jade;
    static eMaterial obsidian;
    static eMaterial pearl;
    static eMaterial ruby;
    static eMaterial turquoise;
    static eMaterial brass;
    static eMaterial bronze;
    static eMaterial chrome;
    static eMaterial copper;
    static eMaterial gold;
    static eMaterial silver;
    static eMaterial black_plastic;
    static eMaterial cyan_plastic;
    static eMaterial green_plastic;
    static eMaterial red_plastic;
    static eMaterial white_plastic;
    static eMaterial yellow_plastic;
    static eMaterial black_rubber;
    static eMaterial cyan_rubber;
    static eMaterial green_rubber;
    static eMaterial red_rubber;
    static eMaterial white_rubber;
    static eMaterial yellow_rubber;

    // function to mix two materials with a proportion
    static eMaterial mix(eMaterial m1, eMaterial m2, float mix = 0.5f);
    // converter
    static mMaterial toMMaterial(eMaterial m);
} eMaterial;

#endif
