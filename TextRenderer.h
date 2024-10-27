// TextRenderer.h
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class Shader;

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class TextRenderer {
public:
    TextRenderer(const std::string& fontPath, unsigned int width, unsigned int height);
    ~TextRenderer();

    void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

private:
    std::map<GLchar, Character> Characters;
    FT_Face face;
    FT_Library ft;
    unsigned int VAO, VBO;

    void LoadCharacters(const std::string& fontPath);
};

#endif
