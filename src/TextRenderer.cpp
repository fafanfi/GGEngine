#include "TextRenderer.h"
#include "macros.h"
#include "TextShader.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <stdexcept>

FT_Library *TextRenderer::ft = nullptr;

void TextRenderer::init() {
    static FT_Library ftT;

    if (FT_Init_FreeType(&ftT)) {
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    TextRenderer::ft = &ftT;
}

TextRenderer::TextRenderer(const char* font) : height(48), proj(glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT)) {
    loadFont(font);
}

TextRenderer::TextRenderer(int height, const char* font) : height(height), proj(glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT)) {
    loadFont(font);
}

void TextRenderer::loadFont(std::string path) {
    FT_Face fontFace;

    if (FT_New_Face(*TextRenderer::ft, path.c_str(), 0, &fontFace)) {
        throw std::runtime_error(std::string("Failed to load font\n"));
    }

    // set height and width
    FT_Set_Pixel_Sizes(fontFace, 0, height); // 0 for width because it will dynamically resize
    // load texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable the byte-alignment restriction so can use 1 byte for each pixel

    for (unsigned char c = 0; c < 128; c++) {
        // load glyph
        if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
            LOG("--------------- Failed to load Glyph");
            continue;
        }

        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);

        // set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // store character texture for use
        chars[c] = {
            texture,
            glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
            glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
            fontFace->glyph->advance.x
        };
    }

    // freeup memory
    FT_Done_Face(fontFace);
    FT_Done_FreeType(*TextRenderer::ft);

    vao.bind();
    vbo.bind(NULL, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW); // 2D quad requires 6 vertices of 4 float (2 for pos, 2 for tex coords)
    vao.setVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    vao.unbind();
    vbo.unbind();
}

void TextRenderer::begin() {
    vao.bind();
    vbo.bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextRenderer::render(TextShader &shader, std::string text, float x, float y, float scale, glm::vec3 color) {
    shader.useProgram();
    shader.setUniforms(proj, 0, color);
    glActiveTexture(GL_TEXTURE0);

    // go through all characters of string
    for (int i = 0, len = text.size(); i < len; i++) {
        Character c = chars[text[i]];
        float xPos = x + c.bearing.x * scale;
        float yPos = y - (c.size.y - c.bearing.y) * scale; // characters might need to be shfited below baseline

        float width = c.size.x * scale;
        float height = c.size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { xPos,     yPos + height,   0.0f, 0.0f },
            { xPos,     yPos,       0.0f, 1.0f },
            { xPos + width, yPos,       1.0f, 1.0f },

            { xPos,     yPos + height,   0.0f, 0.0f },
            { xPos + width, yPos,       1.0f, 1.0f },
            { xPos + width, yPos + height,   1.0f, 0.0f }
        };

        // setup texture
        glBindTexture(GL_TEXTURE_2D, c.textureId);

        // update VBO data
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//        x += (c.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        x += (c.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void TextRenderer::end() {
    vao.unbind();
    vbo.unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void TextRenderer::cleanUp() {
//    FT_Done_FreeType(*TextRenderer::ft);
    vao.deleteVertexArray();
    vbo.deleteBuffer();
}
