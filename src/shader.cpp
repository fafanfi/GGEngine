#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <cstring>

std::string Shader::loadShaderSource(const char* fileName) {
    std::ifstream fileStream;
    std::stringstream sStream;
    std::string ret = "";

    fileStream.open(fileName);

    if (fileStream.is_open()) {
        sStream << fileStream.rdbuf();
        ret = sStream.str();
    } else {
        throw std::runtime_error(std::string("gagal membuka file ").append(fileName).append("\n"));
    }

    fileStream.close();
    return ret;
}

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    std::string vsss = loadShaderSource(vertexShaderSource);
    std::string fsss = loadShaderSource(fragmentShaderSource);
    const GLchar* vssc = vsss.c_str();
    const GLchar* fssc = fsss.c_str();

    // create vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vssc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, infoLogSize, NULL, infoLog);
        throw std::runtime_error(std::string("kompilasi vertex shader gagal\n").append(infoLog));
    }

    // create fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fssc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        strcpy(infoLog, "");
        glGetShaderInfoLog(fragmentShader, infoLogSize, NULL, infoLog);
        throw std::runtime_error(std::string("kompilasi fragment shader gagal\n").append(infoLog));
    }

    // create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        strcpy(infoLog, "");
        glGetProgramInfoLog(shaderProgram, infoLogSize, NULL, infoLog);
        throw std::runtime_error(std::string("gagal link shader program\n").append(infoLog));
    }

//    deleteShaders(); // should call in child
}

void Shader::useProgram() {
    glUseProgram(shaderProgram);
}

void Shader::deleteVertexShader() {
    glDeleteShader(vertexShader);
}

void Shader::deleteFragmentShader() {
    glDeleteShader(fragmentShader);
}

void Shader::deleteShaders() {
    deleteVertexShader();
    deleteFragmentShader();
}

void Shader::deleteProgram() {
    glDeleteProgram(shaderProgram);
}

void Shader::cleanUp() {
    deleteProgram();
}
