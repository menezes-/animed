#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "../OpenGL/Shader.hpp"
#include "Light.hpp"

class PointLight final : public Light {
    friend class Scene;

public:

    void applyUniforms(Shader &shader) const override;

private:
    //representa o nome do array que contem as luzes
    std::string lightContainer{"pointLights"};

    PointLight(glm::vec3 position, int id);

    static std::unique_ptr<PointLight> create(glm::vec3 position, int id);
};
