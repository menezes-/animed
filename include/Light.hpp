#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.hpp"

struct Attenuation {
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
};

enum class LightType : short {
    SPOT,
    POINT
};

class Light {
public:

    Light(LightType type, glm::vec3 position, int id);

    glm::vec3 getPosition() const;

    void setPosition(glm::vec3 position);

    glm::vec3 getAmbient() const;

    void setAmbient(glm::vec3 ambient);

    glm::vec3 getDiffuse() const;

    void setDiffuse(glm::vec3 diffuse);

    glm::vec3 getSpecular() const;

    void setSpecular(glm::vec3 specular);

    Attenuation getAttenuation() const;

    LightType getType() const;

    void setAttenuation(Attenuation attenuation);

    int getId() const;

    virtual void applyUniforms(Shader &shader) const = 0;

    virtual ~Light() {};
protected:

    const LightType type;

    glm::vec3 position;

    int id;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Attenuation attenuation;


    /**
     * inicializa os valores padrões da luz
     * marcada como virtual para que as classes de luz derivada possam implementar seus próprios valores
     * default
     */
    virtual void initDefaults();

};

