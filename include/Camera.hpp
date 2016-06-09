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
    Camera(const glm::vec3 &position, const glm::vec3 &up, GLfloat FOV, GLfloat aspect);

    Camera(const glm::vec3 &position, GLfloat FOV, GLfloat aspect);

    Camera(const glm::vec3 &position);

    GLfloat getFOV() const;

    void setFOV(GLfloat zoom);

    GLfloat getMouseSensitivity() const;

    void setMouseSensitivity(GLfloat mouseSensitivity);

    GLfloat getMovementSpeed() const;

    void setMovementSpeed(GLfloat movementSpeed);

    GLfloat getPitch() const;

    void setPitch(GLfloat pitch);

    GLfloat getYaw() const;

    void setYaw(GLfloat yaw);

    GLfloat getMaxFov() const;

    GLfloat getAspect() const;

    void setAspect(GLfloat aspect);

    void setMaxFov(GLfloat maxFov);


    GLfloat getZNear() const;

    void setZNear(GLfloat zNear);

    GLfloat getZFar() const;

    void setZFar(GLfloat zFar);

    // util para alterar zoom ao fazer scroll do mouse
    void updateFovOffset(GLfloat yoffset);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    glm::mat4 getProjectionViewMatrix() const;

    void move(CameraMovement direction, GLfloat deltaTime);

    /**
     * Rotaciona a camera conforme os movimentos da camera
     */
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, bool constrainPitch = true);


private:
    const GLfloat DEFAULT_YAW = -90.0f;
    const GLfloat DEFAULT_PITCH = 0.0f;
    const GLfloat DEFAULT_SPEED = 3.0f;
    const GLfloat DEFAULT_MOUSE_SENSITIVITY = 0.25f; // se refera sensibilidade do mouse
    const GLfloat DEFAULT_FOV = 45.0f;
    const GLfloat DEFAULT_ASPECT = 16.0f / 9.0f;
    const GLfloat DEFAULT_ZNEAR = 0.1f;
    const GLfloat DEFAULT_ZFAR = 1000.0f;
    const glm::vec3 DEFAULT_UP_VECTOR = glm::vec3{0, 1, 0};

    // valor maximo para o fov:
    GLfloat maxFov;

    // atributos da camera
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 upVector;
    glm::vec3 right;

    // angulos
    GLfloat yaw = DEFAULT_YAW;
    GLfloat pitch = DEFAULT_PITCH;

    // opções da camera
    GLfloat movementSpeed = DEFAULT_SPEED;
    GLfloat mouseSensitivity = DEFAULT_MOUSE_SENSITIVITY;
    GLfloat FOV = DEFAULT_FOV;

    // perspectiva
    GLfloat aspect = DEFAULT_ASPECT;
    GLfloat zNear = DEFAULT_ZNEAR;
    GLfloat zFar = DEFAULT_ZFAR;


    /**
     * Atualiza o vetor front da câmera, levando em consideração os angulos yaw e pitch
     * Também re-calcula o vetor right e up.
     */
    void updateViewVectors();


};
