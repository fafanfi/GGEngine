#include "texture.h"
#include "utils.h"
#include "model.h"

#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::vector<Texture> Texture::loadedTextures = {};

Texture::Texture(const char* path, GLenum unit, aiTextureType type) : unit(unit), type(type) {
    std::string fileName = Utils::getFileName(path);
    Texture *loadedTexture;
    bool textureLoaded = false;

    for (Texture &lt : Texture::loadedTextures) {
        if (lt.name == fileName) {
            loadedTexture = &lt;
            textureLoaded = true;
            break;
        }
    }

    if (textureLoaded) {
        id = loadedTexture->id;
        unit = loadedTexture->unit;
        type = loadedTexture->type;
        width = loadedTexture->width;
        height = loadedTexture->height;
        colChannel = loadedTexture->colChannel;
        name = fileName;
    } else {
    //    stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path, &width, &height, &colChannel, 0);

        if (!data) {
            throw std::runtime_error(std::string("gagal load texture: ").append(stbi_failure_reason()));
        }

        GLenum colorMode;

        switch (colChannel) {
        case 1:
            colorMode = GL_RED;
            break;
        case 4:
            colorMode = GL_RGBA;
            break;
        default:
            colorMode = GL_RGB;
            break;
        }

        glGenTextures(1, &id);
    //    glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, id);

        // fastest: nearest, best: linear
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // used when image resolution not square?
    //    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    //    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    //    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    //    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        unbind();
        // delete texture data
        stbi_image_free(data);

        name = fileName;
        Texture::loadedTextures.push_back(*this);
    }
}

void Texture::activate() {
    glActiveTexture(unit);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture() {
    glDeleteTextures(1, &id);
}
