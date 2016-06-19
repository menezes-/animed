#include <fmt/format.h>
#include "../include/SpotLight.hpp"


SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, int id) : Light(LightType::POINT, position, id),
                                                                        direction{direction} {

    initDefaults();
}

void SpotLight::applyUniforms(Shader &shader) const {
        std::string format = "{}.{}";
        shader.setUniform3f(fmt::format(format, lightContainer, "position"), position);
        shader.setUniform3f(fmt::format(format, lightContainer, "direction"), direction);
        shader.setUniform1f(fmt::format(format, lightContainer, "cutOff"), cutOffCosine);
        shader.setUniform1f(fmt::format(format, lightContainer, "outerCutOff"), outerCutOffCosine);

        shader.setUniform3f(fmt::format(format, lightContainer, "ambient"), ambient);
        shader.setUniform3f(fmt::format(format, lightContainer, "diffuse"), diffuse);
        shader.setUniform3f(fmt::format(format, lightContainer, "specular"), specular);
        shader.setUniform1f(fmt::format(format, lightContainer, "constant"), attenuation.constant);
        shader.setUniform1f(fmt::format(format, lightContainer, "linear"), attenuation.linear);
        shader.setUniform1f(fmt::format(format, lightContainer, "quadratic"), attenuation.quadratic);

}

void SpotLight::setCutOffAngle(GLfloat cutOff) {
    this->cutOffCosine = glm::cos(glm::radians(cutOff));
}

void SpotLight::setOuterCutOffAngle(GLfloat outerCutOffCosine) {
    this->outerCutOffCosine = glm::cos(glm::radians(outerCutOffCosine));
}

void SpotLight::initDefaults() {
    Light::initDefaults();
    setCutOffAngle(12.5f);
    setOuterCutOffAngle(17.5f);
}

bool SpotLight::isSingleLight() const {
    return singleLight;
}

void SpotLight::setSingleLight(bool singleLight) {
    this->singleLight = singleLight;
}

std::unique_ptr<SpotLight> SpotLight::create(glm::vec3 position, glm::vec3 direction, int id) {
    return std::unique_ptr<SpotLight>(new SpotLight(position, direction, id));
}

glm::vec3 SpotLight::getDirection() const {
    return direction;
}

void SpotLight::setDirection(glm::vec3 direction) {
    SpotLight::direction = direction;
}
