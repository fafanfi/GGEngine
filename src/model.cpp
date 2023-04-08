#include "model.h"
#include "texture.h"
#include "macros.h"
#include "utils.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <stdexcept>

Model::Model(const char* path, BoundTypes boundType) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      throw std::runtime_error(std::string("Could not load model: ").append(importer.GetErrorString()));
    }

    processNode(scene->mRootNode, scene, boundType);
}

void Model::processNode(aiNode* node, const aiScene* scene, BoundTypes boundType) {
    LOG(std::string("node id: ").append(node->mName.C_Str()));
    // process all meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      LOG(std::string("mesh id: ").append(mesh->mName.C_Str()));
      meshes.push_back(processMesh(mesh, scene, boundType));
    }

    // process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, boundType);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, BoundTypes boundType) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    mMaterial material = {glm::vec4(0.0f), glm::vec4(.0f), glm::vec4(0.0f), 0};

    BoundingRegion boundingRegion(boundType);
    glm::vec3 min((float) (~0)); // min point = max float
    glm::vec3 max(-(float) (~0)); // max point = min float

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // position
        vertex.pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // getting min and max
        for (int j = 0; j < 3; j++) {
            // if smaller than min
            if (vertex.pos[j] < min[j]) min[j] = vertex.pos[j];
            // if larger than max
            if (vertex.pos[j] > max[j]) max[j] = vertex.pos[j];
        }

        // normal vectors
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        // textures
        if (mesh->mTextureCoords[0]) {
            vertex.textCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.textCoord = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    // process min/max for BR
    if (boundType == BoundTypes::AABB) {
        // assign max and min
        boundingRegion.min = min;
        boundingRegion.max = max;
    } else {
        // calculate max distance from the center
        boundingRegion.center = BoundingRegion(min, max).calculateCenter();
        float maxRadiusSquared = 0.0f; // distance for this vertex

        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            float radiusSquared = 0.0f; // distance for this vertex

            for(int j = 0; j < 3; j++) {
                radiusSquared += (vertices[i].pos[j] - boundingRegion.center[j]) * (vertices[i].pos[j] - boundingRegion.center[j]);
            }

            if (radiusSquared > maxRadiusSquared) {
                // found new squared radius
                // a^2 > b^2 --> |a| > |b|
                maxRadiusSquared = radiusSquared;
            }
        }

        boundingRegion.radius = sqrt(maxRadiusSquared);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        // diffuse maps
        std::vector<Texture> diffuseMaps = loadTextures(mat, aiTextureType_DIFFUSE, 0);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

//        LOG("LOADING MODEL");
//        LOG(std::string("diffuseMaps size: ") + std::to_string(diffuseMaps.size()));

        // specular maps
        std::vector<Texture> specularMaps = loadTextures(mat, aiTextureType_SPECULAR, diffuseMaps.size());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

//        LOG("LOADING MODEL");
//        LOG(std::string("specularMaps size: ") + std::to_string(specularMaps.size()));

        aiColor4D diff;
        aiColor4D spec;
        ai_real shininess;

//        if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diff) == AI_SUCCESS) {
        if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diff) == AI_SUCCESS) {
            material.diffuse.r = diff.r;
            material.diffuse.g = diff.g;
            material.diffuse.b = diff.b;
            material.diffuse.a = diff.a;
//            LOG(material.diffuse.r);
//            LOG(material.diffuse.g);
//            LOG(material.diffuse.b);
        } else {
//            throw std::runtime_error("error getting material diffuse");
            LOG("------------------- error getting material diffuse");
        }

        if (mat->Get(AI_MATKEY_COLOR_SPECULAR, spec) == AI_SUCCESS) {
            material.specular.r = spec.r;
            material.specular.g = spec.g;
            material.specular.b = spec.b;
            material.specular.a = spec.a;
//            LOG(material.specular.r);
//            LOG(material.specular.g);
//            LOG(material.specular.b);
        } else {
//            throw std::runtime_error("error getting material specular");
            LOG("------------------- error getting material specular");
        }

        if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            material.shininess = shininess / 128; // karna di shader di kali 128 (default untuk object non model )
//            LOG(material.shininess);
        } else {
//            throw std::runtime_error("error getting material shininess");
            LOG("------------------- error getting material shininess");
        }

    }

    return Mesh(vertices, indices, textures, material, boundingRegion);
}

std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType type, int currentLoadedTextureCount) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString texture;
        material->GetTexture(type, i, &texture);

        bool skip = false;

        for (Texture &txt : Texture::loadedTextures) {
            std::string fileName = Utils::getFileName(texture.C_Str());

            if (std::strcmp(txt.name.c_str(), fileName.c_str()) == 0) {
                textures.push_back(txt);
                skip = true;
                break;
            }
        }

        if (!skip) {
            textures.push_back(Texture(texture.C_Str(), GL_TEXTURE0 + currentLoadedTextureCount++, type));
//            loadedTextures.push_back(tmp);
        }
    }

    return textures;
}
