#include "../include/Camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, GLfloat FOV, GLfloat aspect) : position(position),
                                                                                              upVector(up),
                                                                                              FOV(FOV),
                                                                                              aspect(aspect) {
    updateViewVectors();
}

Camera::Camera(const glm::vec3 &position, GLfloat FOV, GLfloat aspect) : Camera(position, DEFAULT_UP_VECTOR, FOV,
                                                                                aspect) { }

Camera::Camera(const glm::vec3 &position) : Camera(position, DEFAULT_FOV, DEFAULT_ASPECT) { }

GLfloat Camera::getFOV() const {
    return FOV;
}

void Camera::setFOV(GLfloat zoom) {
    this->FOV = zoom;
}

GLfloat Camera::getMouseSensitivity() const {
    return mouseSensitivity;
}

void Camera::setMouseSensitivity(GLfloat mouseSensitivity) {
    this->mouseSensitivity = mouseSensitivity;
}

GLfloat Camera::getMovementSpeed() const {
    return movementSpeed;
}

void Camera::setMovementSpeed(GLfloat movementSpeed) {
    this->movementSpeed = movementSpeed;
}

GLfloat Camera::getPitch() const {
    return pitch;
}

void Camera::setPitch(GLfloat pitch) {
    this->pitch = pitch;
}

GLfloat Camera::getYaw() const {
    return yaw;
}

void Camera::setYaw(GLfloat yaw) {
    this->yaw = yaw;
}

void Camera::updateViewVectors() {

    glm::vec3 frontTmp;
    frontTmp.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    frontTmp.y = glm::sin(glm::radians(pitch));
    frontTmp.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(frontTmp);
    //normaliza os vetores, pois quanto mais os comprimentos chegam perto de zero mais a camera olha pra cima ou pra baixo deixando o movimento lento
    right = glm::normalize(glm::cross(front,
                                      upVector));
    upVector = glm::normalize(glm::cross(right, front));

}

void Camera::updateFovOffset(GLfloat yoffset) {

    if (FOV >= 1.0f && FOV <= maxFov) {
        FOV -= yoffset;
    }
    if (FOV <= 1.0f) {
        FOV = 1.0f;
    }
    if (FOV >= maxFov) {
        FOV = maxFov;
    }
}

void Camera::setMaxFov(GLfloat maxFov) {
    this->maxFov = maxFov;
}

GLfloat Camera::getMaxFov() const {
    return maxFov;
}

GLfloat Camera::getAspect() const {
    return aspect;
}

void Camera::setAspect(GLfloat aspect) {
    this->aspect = aspect;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, upVector);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(FOV, aspect, zNear, zFar);
}

glm::mat4 Camera::getProjectionViewMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}


GLfloat Camera::getZNear() const {
    return zNear;
}

void Camera::setZNear(GLfloat zNear) {
    this->zNear = zNear;
}

GLfloat Camera::getZFar() const {
    return zFar;
}

void Camera::setZFar(GLfloat zFar) {
    this->zFar = zFar;
}

void Camera::move(CameraMovement direction, GLfloat deltaTime) {

    GLfloat velocity = movementSpeed * deltaTime;
    if (direction == FORWARD) {
        position += front * velocity;
    }
    if (direction == BACKWARD) {
        position -= front * velocity;
    }
    if (direction == LEFT) {
        position -= right * velocity;
    }
    if (direction == RIGHT) {
        position += right * velocity;
    }

}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, bool constrainPitch) {


    yaw += mouseSensitivity * xoffset;
    pitch += mouseSensitivity * yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    updateViewVectors();
}
