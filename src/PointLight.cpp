#include <fmt/format.h>
#include "../include/PointLight.hpp"
#include "../include/utils.hpp"

void Lights::draw(Shader &shader) {
    for (const auto &light:lights) {
        light.applyUniforms(shader);
    }
}

PointLight::PointLight(glm::vec3 position) : position{position} {
    initDefaults();
}

PointLight::PointLight(glm::vec3 position, int id, std::string lightContainer) : position{position}, id{id},
                                                                                 lightContainer{lightContainer} {
    initDefaults();
}

void PointLight::initDefaults() {

    diffuse = constants::LIGHTING_DIFFUSE;
    specular = constants::LIGHTING_SPECULAR;
    ambient = constants::LIGHTING_AMBIENT;

    attenuation = constants::LIGHTING_ATTENUATION;
}

glm::vec3 PointLight::getPosition() const {
    return position;
}

glm::vec3 PointLight::getAmbient() const {
    return ambient;
}

void PointLight::setAmbient(glm::vec3 ambient) {
    this->ambient = ambient;
}

glm::vec3 PointLight::getDiffuse() const {
    return diffuse;
}

void PointLight::setDiffuse(glm::vec3 diffuse) {
    this->diffuse = diffuse;
}

glm::vec3 PointLight::getSpecular() const {
    return specular;
}

void PointLight::setSpecular(glm::vec3 specular) {
    this->specular = specular;
}

void PointLight::setAttenuation(Attenuation attenuation) {
    this->attenuation = attenuation;
}

Attenuation PointLight::getAttenuation() const {
    return attenuation;
}

void PointLight::applyUniforms(Shader &shader) const {
    static std::string format = "{}[{}].{}";
    auto name = fmt::format(format, lightContainer, id, "position");
    shader.setUniform3f(name, position);
    auto uniform_name = fmt::format(format, lightContainer, id, "ambient");
    shader.setUniform3f(uniform_name, ambient);
    shader.setUniform3f(fmt::format(format, lightContainer, id, "diffuse"), diffuse);
    shader.setUniform3f(fmt::format(format, lightContainer, id, "specular"), specular);
    shader.setUniform1f(fmt::format(format, lightContainer, id, "constant"), attenuation.constant);
    shader.setUniform1f(fmt::format(format, lightContainer, id, "linear"), attenuation.linear);
    shader.setUniform1f(fmt::format(format, lightContainer, id, "quadratic"), attenuation.quadratic);
}

int PointLight::getId() const {
    return id;
}

void PointLight::setId(int id) {
    this->id = id;
}

std::string PointLight::getLightContainer() const {
    return lightContainer;
}

void PointLight::setLightContainer(std::string lightContainer) {
    this->lightContainer = lightContainer;
}
