#include "../include/Camera.hpp"
#include "../include/Constants.hpp"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(const glm::vec3 &position) : position{position}, worldUp{0.0f, 1.0f, 0.0f},
                                            movementSpeed{constants::CAMERA_SPEED}, pitch{constants::CAMERA_PITCH},
                                            yaw{constants::CAMERA_YAW}, front{0.0f, 0.0f, -1.0f},
                                            nearPlane{constants::CAMERA_NEAR_PLANE},
                                            farPlane{constants::CAMERA_FAR_PLANE}, fov{constants::CAMERA_FOV},
                                            maxFov{constants::CAMERA_MAX_FOV}, aspect{constants::CAMERA_ASPECT},
                                            mouseSensitivity{constants::CAMERA_MOUSE_SENSITIVTY} {
    updateCameraVectors();
}

GLfloat Camera::getYaw() const {
    return yaw;
}

void Camera::setYaw(GLfloat yaw) {
    this->yaw = yaw;
    updateCameraVectors();
}

GLfloat Camera::getPitch() const {
    return pitch;
}

void Camera::setPitch(GLfloat pitch) {
    this->pitch = pitch;
    updateCameraVectors();
}

GLfloat Camera::getMovementSpeed() const {
    return movementSpeed;
}

void Camera::setMovementSpeed(GLfloat movementSpeed) {
    this->movementSpeed = movementSpeed;
}

GLfloat Camera::getMouseSensitivity() const {
    return mouseSensitivity;
}

void Camera::setMouseSensitivity(GLfloat mouseSensitivity) {
    this->mouseSensitivity = mouseSensitivity;
}

void Camera::updateCameraVectors() {

    // calcula o novo front vector
    glm::vec3 front;
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    right = glm::normalize(glm::cross(front,
                                      worldUp));
    up = glm::normalize(glm::cross(right, front));

}


GLfloat Camera::getNearPlane() const {
    return nearPlane;
}

void Camera::setNearPlane(GLfloat nearPlane) {
    this->nearPlane = nearPlane;
}

GLfloat Camera::getFarPlane() const {
    return farPlane;
}

void Camera::setFarPlane(GLfloat farPlane) {
    this->farPlane = farPlane;
}

GLfloat Camera::getAspect() const {
    return aspect;
}

void Camera::setAspect(GLfloat aspect) {
    this->aspect = aspect;
}

GLfloat Camera::getFov() const {
    return fov;
}

void Camera::setFov(GLfloat fov) {
    this->fov = glm::clamp(fov, 1.0f, maxFov);
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, bool constrainPitch) {

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;

    pitch += yoffset;

    if (constrainPitch) {
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void Camera::zoom(GLfloat yoffset) {
    setFov(fov - yoffset);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::offsetPosition(const glm::vec3 &offset) {
    position += offset;
}


GLfloat Camera::getMaxFov() const {
    return maxFov;
}

void Camera::setMaxFov(GLfloat maxFov) {
    this->maxFov = maxFov;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(fov, aspect, nearPlane, farPlane);
}

glm::mat4 Camera::getMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}

void Camera::move(CameraMovement direction, GLfloat deltaTime) {
    GLfloat velocity = movementSpeed * deltaTime;

    switch (direction) {

        case FORWARD:
            position += front * velocity;
            break;
        case BACKWARD:
            position -= front * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
    }

}


glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::setPosition(const glm::vec3 &position) {
    Camera::position = position;
}

void Camera::applyUniforms(Shader &shader) const {
    shader.setMatrix4fv("projection", glm::value_ptr(getProjectionMatrix()));
    shader.setMatrix4fv("view", glm::value_ptr(getViewMatrix()));
}

glm::vec3 Camera::getFront() const {
    return front;
}

