#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.hpp"
#include "ShaderLoader.hpp"


struct Attenuation {
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
};

class PointLight;

struct Lights {
    std::vector<PointLight> lights;

    void draw(Shader &shader);

};

class PointLight {
    friend class Scene;
public:
    PointLight(glm::vec3 position);

    glm::vec3 getPosition() const;

    glm::vec3 getAmbient() const;

    void setAmbient(glm::vec3 ambient);

    glm::vec3 getDiffuse() const;

    void setDiffuse(glm::vec3 diffuse);

    glm::vec3 getSpecular() const;

    void setSpecular(glm::vec3 specular);

    Attenuation getAttenuation() const;

    void setAttenuation(Attenuation attenuation);

    int getId() const;

    void setId(int id);

    std::string getLightContainer() const;

    void setLightContainer(std::string lightContainer);

    void applyUniforms(Shader& shader) const;

private:
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Attenuation attenuation;


    //representa a posição no array de luzes do shader
    int id;

    //representa o nome do array que contem as luzes
    std::string lightContainer;

    PointLight(glm::vec3 position, int id, std::string lightContainer);

    void initDefaults();
};