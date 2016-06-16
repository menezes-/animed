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
#include <functional>
#include <memory>

class Scene {
public:
    Scene(const Config &config, Camera &camera);

    void draw();

    std::size_t getMaxLights() const;

    void setMaxLights(std::size_t maxLights);

    bool addLight(PointLight light);

    void newModelInstance(const std::string &objectName);

    void newModelInstance(const std::string &objectName, const Transform &transform);

    void setRenderLights(bool renderLights);

private:
    const Config &config;
    Camera &camera;
    ShaderLoader shaderLoader;
    std::unique_ptr<Model> lampModel;

    bool renderLights{false};


    TextureLoader textureLoader{};
    std::string lightContainerName{"pointLights"};

    std::vector<PointLight> lights;

    std::vector<std::pair<Model, std::reference_wrapper<Shader> > > models;

    std::size_t maxLights = constants::LIGHTING_MAX_LIGHTS;

    bool addLight(const LightConfig &light, int id);

    template<class T>
    void applyUniforms(const T &obj, Shader &shader) {
        obj.applyUniforms(shader);
    }

};
