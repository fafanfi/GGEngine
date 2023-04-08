#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "GameObject.h"
#include "texture.h"
#include "material.h"

class DefaultShader2D;
class OrthographicCamera;

class Rectangle : public GameObject {
    glm::vec4 color;

public:
    Rectangle(int width, int height, eMaterial material);
    Rectangle(int width, int height, glm::vec4 color);
    Rectangle(Texture texture);
    void render(DefaultShader2D &shader, OrthographicCamera &camera);
};

#endif // RECTANGLE_H
