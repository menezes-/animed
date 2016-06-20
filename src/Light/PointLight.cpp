#include "../include/Light/PointLight.hpp"
#include "../include/Constants.hpp"

#include <fmt/format.h>

PointLight::PointLight(glm::vec3 position, int id) : Light(LightType::POINT, position, id) {
    Light::initDefaults();
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


std::unique_ptr<PointLight> PointLight::create(glm::vec3 position, int id) {
    return std::unique_ptr<PointLight>(new PointLight(position, id));
}
