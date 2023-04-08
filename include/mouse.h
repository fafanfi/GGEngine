#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>

class Mouse {
    Mouse();

public:
    inline static double x = 0;
    inline static double y = 0;
    inline static double lastX = 0;
    inline static double lastY = 0;
    inline static double offsetX = 0;
    inline static double offsetY = 0;

    static void cursorPosCallback(GLFWwindow* Window, double _x, double _y);
};

#endif // MOUSE_H
