#include "camera.h"
#include "macros.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

//////////////////////////////////////////////////// PERSPECTIVE CAMERA

// constructor with vectors
PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
    View = GetViewMatrix();
    Proj = glm::perspective(glm::radians(ZOOM), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, .01f, 2000.f);
//    Proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT, 0.01f, 1000.0f);
}

// constructor with scalar values
PerspectiveCamera::PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
    View = GetViewMatrix();
    Proj = glm::perspective(glm::radians(ZOOM), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, .01f, 2000.f);
//    Proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT, 0.01f, 1000.0f);
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 PerspectiveCamera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Direction, Up);
}

void PerspectiveCamera::UpdateView() {
    View = GetViewMatrix();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void PerspectiveCamera::Move(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Direction * velocity;
    if (direction == BACKWARD)
        Position -= Direction * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += Up * velocity;
    if (direction == DOWN)
        Position -= Up * velocity;

    UpdateView();
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void PerspectiveCamera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch)
{
    xoffset *= (MouseSensitivity * deltaTime);
    yoffset *= (MouseSensitivity * deltaTime);

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
    UpdateView();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void PerspectiveCamera::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;

    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void PerspectiveCamera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 dir;

    dir.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    dir.y = sin(glm::radians(Pitch));
    dir.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Direction = glm::normalize(dir);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Direction, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Direction));
}

//////////////////////////////////////////////////// ORTHOGRAPHIC CAMERA

OrthographicCamera::OrthographicCamera() {
    Position = glm::vec3(0.0f);
    Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    Up = glm::vec3(0.0f, 0.0f, 1.0f);
    View = GetViewMatrix();
    Proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT, 0.01f, 1.0f);
}

// constructor with vectors
//OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Direction(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
//{
//    Position = position;
//    WorldUp = up;
//    Yaw = yaw;
//    Pitch = pitch;
//    updateCameraVectors();
//    View = GetViewMatrix();
//    Proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT);
//}
//
//// constructor with scalar values
//OrthographicCamera::OrthographicCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Direction(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
//{
//    Position = glm::vec3(posX, posY, posZ);
//    WorldUp = glm::vec3(upX, upY, upZ);
//    Yaw = yaw;
//    Pitch = pitch;
//    updateCameraVectors();
//    View = GetViewMatrix();
//    Proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT);
//}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 OrthographicCamera::GetViewMatrix()
{
    return glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    return glm::lookAt(Position, Position + Direction, Up);

}

//void OrthographicCamera::UpdateView() {
//    View = GetViewMatrix();
//}
