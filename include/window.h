#ifndef WINDOW_H
#define WINDOW_H

// order important (before GLFW/glfw3.h)
#include <glad/glad.h>
//////////////////
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Window {
    public:
        GLFWwindow* window;
        int width;
        int height;

        Window(int width, int height);
        void makeContextCurrent();
};

#endif // WINDOW_H
