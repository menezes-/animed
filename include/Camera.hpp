#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum CameraMovement : short {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 cameraUp;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;

    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

};
