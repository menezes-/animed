#include "../include/Light.hpp"
#include "../include/Constants.hpp"

Light::Light(LightType type, glm::vec3 position, int id) : type{type}, position{position}, id{id} {

}

glm::vec3 Light::getPosition() const {
    return position;
}

void Light::setPosition(glm::vec3 position) {
    this->position = position;
}

glm::vec3 Light::getAmbient() const {
    return ambient;
}

void Light::setAmbient(glm::vec3 ambient) {
    this->ambient = ambient;
}

glm::vec3 Light::getDiffuse() const {
    return diffuse;
}

void Light::setDiffuse(glm::vec3 diffuse) {
    this->diffuse = diffuse;
}

glm::vec3 Light::getSpecular() const {
    return specular;
}

void Light::setSpecular(glm::vec3 specular) {
    this->specular = specular;
}

Attenuation Light::getAttenuation() const {
    return attenuation;
}

void Light::setAttenuation(Attenuation attenuation) {
    this->attenuation = attenuation;
}

void Light::initDefaults() {
    diffuse = constants::LIGHTING_DIFFUSE;
    specular = constants::LIGHTING_SPECULAR;
    ambient = constants::LIGHTING_AMBIENT;

    attenuation = constants::LIGHTING_ATTENUATION;

}

LightType Light::getType() const {
    return type;
}

int Light::getId() const {
    return id;
}

