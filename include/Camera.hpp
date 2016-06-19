#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"


enum CameraMovement : short {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:


    Camera(const glm::vec3 &position);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    void processMouseMovement(GLfloat xoffset, GLfloat yoffset, bool constrainPitch = true);

    void zoom(GLfloat yoffset);

    void move(CameraMovement direction, GLfloat deltaTime);

    void offsetPosition(const glm::vec3 &offset);

    /**
     * Retorna a matriz view e projection
     */
    glm::mat4 getMatrix() const;

    GLfloat getYaw() const;

    void setYaw(GLfloat yaw);

    GLfloat getPitch() const;

    void setPitch(GLfloat pitch);

    GLfloat getMovementSpeed() const;

    void setMovementSpeed(GLfloat movementSpeed);

    GLfloat getMouseSensitivity() const;

    void setMouseSensitivity(GLfloat mouseSensitivity);


    GLfloat getNearPlane() const;

    void setNearPlane(GLfloat nearPlane);

    GLfloat getFarPlane() const;

    void setFarPlane(GLfloat farPlane);

    GLfloat getAspect() const;


    GLfloat getFov() const;

    void setFov(GLfloat fov);

    void setAspect(GLfloat aspect);

    GLfloat getMaxFov() const;

    void setMaxFov(GLfloat maxFov);


    glm::vec3 getPosition() const;

    void setPosition(const glm::vec3 &position);

    void applyUniforms(Shader &shader) const;

    glm::vec3 getFront() const;

private:


    glm::vec3 position;
    glm::vec3 front;

    /**
     * Vetor que aponta para o eixo Y positivo da camera
     */
    glm::vec3 up;

    /**
     * Vetor que representa o eixo X positivo da camera
     */
    glm::vec3 right;

    /**
     * Vetor que aponta pra cima no espaço do mundo
     */
    const glm::vec3 worldUp;

    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;

    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;

    // perspectiva
    GLfloat maxFov;
    GLfloat fov;
    GLfloat nearPlane;
    GLfloat farPlane;
    GLfloat aspect;


    void updateCameraVectors();

};
