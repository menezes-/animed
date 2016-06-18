#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

enum class Axis : short {
    X,
    Y,
    Z
};

class Transform {
public:

    /**
     * a rotação é dada em angulos
     */
    void setRotation(GLfloat angle, Axis axis);

    GLfloat getRotation(Axis axis);

    glm::vec3 getScale() const;

    void setScale(glm::vec3 scale);

    glm::vec3 getTranslate() const;

    void setTranslate(glm::vec3 translate);

    /**
     * Aplica as transformações na matrix passada.
     *
     * @param [out] modelMatrix matrix contendo as transformações
     */
    void apply(glm::mat4 &modelMatrix) const;

private:
    GLfloat RX;
    GLfloat RY;
    GLfloat RZ;

    glm::vec3 scale;

    glm::vec3 translate;

};

