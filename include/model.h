#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "bounds.h"
#include "GameObject.h"

class Model : public GameObject {
    void processNode(aiNode* node, const aiScene* scene, BoundTypes boundType);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, BoundTypes boundType);
    std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, int currentLoadedTextureCount);

public:
    Model(const char* path, BoundTypes boundType);
};

#endif // MODEL_H
