#pragma once

#include "Config.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Constants.hpp"
#include "TextureLoader.hpp"
#include "Model.hpp"
#include "ShaderLoader.hpp"
#include <string>
#include <vector>
#include <utility>

class Scene {
public:
    Scene(const Config& config, Camera& camera);

    void draw();

    std::size_t getMaxLights() const;

    void setMaxLights(std::size_t maxLights);

    bool addLight(PointLight light);

private:
    const Config& config;
    Camera& camera;
    std::string lightContainerName{"pointLights"};

    TextureLoader textureLoader{};

    ShaderLoader shaderLoader;

    std::vector<PointLight> lights;

    std::vector<std::pair<Model, Shader&> > models;

    std::size_t maxLights = constants::LIGHTING_MAX_LIGHTS;

    bool addLight(const LightConfig &light, int id);

    void loadModels();

    template <class T>
    void applyUniforms(const T& obj, Shader& shader){
        obj.applyUniforms(shader);
    }

};
