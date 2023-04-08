#ifndef BOUNDS_H
#define BOUNDS_H

#include <glm/glm.hpp>

enum class BoundTypes : unsigned char {
    AABB = 0x00, // axis-algned bounding box
    SPHERE = 0x01 // just for take less memory
};

class BoundingRegion {
    BoundTypes type;

public:
    // sphere values
    glm::vec3 center;
    float radius;
    //bounding box values
    glm::vec3 min;
    glm::vec3 max;

    // initialize with type
    BoundingRegion(BoundTypes type);
    // initialize as sphere
    BoundingRegion(glm::vec3 center, float radius);
    // initialize as AARBB
    BoundingRegion(glm::vec3 min, glm::vec3 max);
    glm::vec3 calculateCenter();
    glm::vec3 calculateDimensions();
    // determine if point inside
    bool containsPoint(glm::vec3 pt);
    // determine if region completely inside
    bool containsRegion(BoundingRegion &br);
    // determine if region intersect (partial containment)
    bool intersectsWith(BoundingRegion &br, glm::vec3& distance);
    // update center position, min and max
    void update(glm::vec3 center, float scale = 1.0f);
};

#endif // BOUNDS_H
