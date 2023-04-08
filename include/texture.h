#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <assimp/scene.h>

#include <vector>
#include <string>

class Texture {
    GLuint id;
    int width;
    int height;
    int colChannel;
    GLenum unit;

public:
    std::string name;
    aiTextureType type;

    static std::vector<Texture> loadedTextures;

    Texture(const char* path, unsigned int unit, aiTextureType type);
    void activate();
    void bind();
    void unbind();
    void deleteTexture();
};

#endif // TEXTURE_H
