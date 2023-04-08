#include "bounds.h"

BoundingRegion::BoundingRegion(BoundTypes type) : type(type) {

}

BoundingRegion::BoundingRegion(glm::vec3 center, float radius) : type(BoundTypes::SPHERE), center(center), radius(radius) {

}

BoundingRegion::BoundingRegion(glm::vec3 min, glm::vec3 max) : type(BoundTypes::AABB), min(min), max(max) {

}

glm::vec3 BoundingRegion::calculateCenter() {
    return (type == BoundTypes::AABB) ? (min + max) / 2.0f : center;
}

glm::vec3 BoundingRegion::calculateDimensions() {
    return (type == BoundTypes::AABB) ? (max - min) : glm::vec3(2.0f * radius);
}

bool BoundingRegion::containsPoint(glm::vec3 pt) {
    if (type == BoundTypes::AABB) {
        // box - point must be larger than man and smaller than max
        return (pt.x >= min.x) && (pt.x <= max.x) && (pt.y >= min.y) && (pt.y <= max.y) && (pt.z >= min.z) && (pt.z <= max.z);
    } else {
        // sphere - distance must be less than radius
        // x^2 + y^2 + z^2 <= r^2
        float distSquared = 0.0f;

        for (int i = 0; i < 3; i++) {
            distSquared += (center[i] - pt[i]) * (center[i] - pt[i]);
        }

        return distSquared <= (radius * radius);
    }
}

bool BoundingRegion::containsRegion(BoundingRegion &br) {
    if (br.type == BoundTypes::AABB) {
        // if br is a box, just has to contain min and max
        return containsPoint(br.min) && containsPoint(br.max);
    } else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
        // if both sphere, combination of distance from centers and br.radius is less than radius
        return glm::length(center - br.center) + br.radius < radius;
    } else {
        // if this is a box and br is a sphere
        if (!containsPoint(br.center)) {
            // center is outside of box
            return false;
        }

        // center inside the box
        /*
          for each axis (x, y, z)
          - if distance to each side is smaller than the radius, return false
        */
        for (int i = 0; i < 3; i++) {
            if (abs(max[i] - br.center[i]) < br.radius || abs(br.center[i] - min[i]) < br.radius) {
                return false;
            }
        }

        return true;
    }
}

bool BoundingRegion::intersectsWith(BoundingRegion &br, glm::vec3& distance) {
    // overlap on all axes
    if (type == BoundTypes::AABB && br.type == BoundTypes::AABB) {
        //both boxes
        glm::vec3 rad = calculateDimensions() / 2.0f; // "radius" of this box
        glm::vec3 radBr = br.calculateDimensions() / 2.0f; // "radius" of br
        glm::vec3 center = calculateCenter(); // center of this box
        glm::vec3 centerBr = br.calculateCenter(); // center of br
        glm::vec3 dist = center - centerBr;
        glm::vec3 absDist = abs(dist);

        for(int i = 0; i < 3; i++) {
            if (absDist[i] > rad[i] + radBr[i]) {
                // no oerlap on this axis
                return false;
            }
        }

        distance = dist;
        // failed to prove wrong on each axis
        return true;
    } else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
        // both spheres - dstance between centers must be less than combined radius

        return glm::length(center - br.center) < (radius + br.radius);
    } else if (type == BoundTypes::SPHERE) {
        // this is a sphere, br is a box

        // determine if sphere is above top, below bottom, etc
        // find distance (squared) to the closest plane
        float distSquared = 0.0f;

        for (int i = 0; i < 3; i++) {
            if (center[i] < br.min[i]) {
                // beyond min
                distSquared += (br.min[i] - center[i]) * (br.min[i] * center[i]);
            } else if (center[i] > br.max[i]) {
                // beyond max
                distSquared += (center[i] - br.max[i]) * (center[i] - br.max[i]);
            }
            // else inside
        }

        return distSquared < (radius * radius);
    } else {
        // this is a box, br is a sphere
        // call algorithm for br (defined in preceding else if block)
        return br.intersectsWith(*this, distance);
    }
}

// update center position, min and max
void BoundingRegion::update(glm::vec3 newCenter, float scale) {
    glm::vec3 c = calculateCenter();
    min += newCenter - c;
    max += newCenter - c;

    // for scaling
    min = newCenter + ((min - newCenter) * scale);
    max = newCenter + ((max - newCenter) * scale);

    center = newCenter;
}
