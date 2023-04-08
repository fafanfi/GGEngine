#include "window.h"
#include <stdexcept>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height) {
    this->width = width;
    this->height = height;

    window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window\n");
    }

}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw "Failed to initialize GLAD";
    }

//    framebuffer_size_callback(window, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
