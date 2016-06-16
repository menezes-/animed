#include <glm/gtc/matrix_transform.hpp>
#include "../include/Transform.hpp"

glm::vec3 Transform::getScale() const {
    return scale;
}

void Transform::setScale(glm::vec3 scale) {
    this->scale = scale;
}

glm::vec3 Transform::getTranslate() const {
    return translate;
}

void Transform::setTranslate(glm::vec3 translate) {
    this->translate = translate;
}

void Transform::setRotation(GLfloat angle, Axis axis) {
    switch (axis) {
        case Axis::X:
            RX = angle;
            break;
        case Axis::Y:
            RY = angle;
            break;
        case Axis::Z:
            RZ = angle;
            break;
    }
}

GLfloat Transform::getRotation(Axis axis) {
    switch (axis) {
        case Axis::X:
            return RX;
        case Axis::Y:
            return RY;
        default:
            return RZ;
    }
}

void Transform::apply(glm::mat4 &modelMatrix) const {
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(RX), glm::vec3{1.0f, 0.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, glm::radians(RY), glm::vec3{0.0f, 1.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, glm::radians(RZ), glm::vec3{0.0f, 0.0f, 1.0f});
    modelMatrix = glm::translate(modelMatrix, translate);
}





