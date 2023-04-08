#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

typedef struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 textCoord;
} Vertex;

#endif // VERTEX_H
