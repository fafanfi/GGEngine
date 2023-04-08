#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>

#include "material.h"
#include "GameObject.h"

class BoundingRegion;

class Cube : public GameObject {
public:
    Cube(eMaterial material);
    BoundingRegion* getBoundingRegion();
    bool intersectsWith(BoundingRegion &br, glm::vec3 &distance);
};

#endif // CUBE_H
