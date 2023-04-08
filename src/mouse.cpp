#include "mouse.h"
#include "macros.h"

#include <string>

void Mouse::cursorPosCallback(GLFWwindow* window, double _x, double _y) {
    x = _x;
    y = _y;

    offsetX = x - lastX;
    offsetY = lastY - y; // inverted

    lastX = x;
    lastY = y;
}
