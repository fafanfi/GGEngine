#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  .1f;
const float SENSITIVITY =  0.2f;
const float ZOOM        =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Direction; // Front
    glm::vec3 Up;
    glm::mat4 View;
    glm::mat4 Proj;
};

class PerspectiveCamera : public Camera {
public:
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    PerspectiveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Move(Camera_Movement direction, float deltaTime);
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch = true);
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    // for update view matrix
    void UpdateView();

private:
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

class OrthographicCamera : public Camera {
public:
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    OrthographicCamera();
//    OrthographicCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
//    OrthographicCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void UpdateView();

private:
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
};

#endif

