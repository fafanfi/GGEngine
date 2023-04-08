#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <string>
#include <map>

#include "vao.h"
#include "vbo.h"

class TextShader;

typedef struct Character {
    unsigned int textureId; // texture id storing character
    glm::ivec2 size; // size of char
    glm::ivec2 bearing; // distance from origin to top left of char
    unsigned int advance; // distance from origin to next origin
} Character;

class TextRenderer {
    int height;
    // FT_Face fontFace;
    std::map<char, Character> chars;
    VAO vao;
    VBO vbo;

    glm::mat4 proj; // projection matrix

    static FT_Library *ft;

    void loadFont(std::string path);

public:
    TextRenderer(const char* font);
    TextRenderer(int height, const char* font);

    static void init();

    void begin();
    void render(TextShader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
    void end();
    void cleanUp();
};

#endif // TEXTRENDERER_H
