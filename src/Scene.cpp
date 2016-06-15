#include <fmt/format.h>
#include "../include/Scene.hpp"

Scene::Scene(const Config &config, Camera &camera) : config(config), camera(camera),
                                                     shaderLoader{config.shaderBasePath} {
    camera.setMouseSensitivity(config.mouseSensitivity);
    camera.setMovementSpeed(config.cameraSpeed);

}

void Scene::draw() {
/*
    for (const auto &m: config.models) {
        const ModelConfig &mc = m.second;
        auto f = shaders.find(mc.shader);
        Shader &shader = f->second;

        //atualiza os valores da camera
        applyUniforms(camera, shader);

        //seta o viewPos pras luzes funcionarem direito
        shader.setUniform3f("viewPos", camera.getPosition());


         // He said LET THERE BE LIGHT....

        for (const auto &light: lights) {
            applyUniforms(light, shader);
        }
        // .. AND THERE WAS LIGHT




    }*/
}

std::size_t Scene::getMaxLights() const {
    return maxLights;
}

void Scene::setMaxLights(std::size_t maxLights) {
    this->maxLights = maxLights;
}

bool Scene::addLight(PointLight light) {
    if (lights.size() >= maxLights) {
        return false;
    }

    lights.push_back(light);
    return true;
}

bool Scene::addLight(const LightConfig &light, int id) {
    auto pointLight = PointLight{light.position, id, lightContainerName};

    pointLight.setAmbient(light.ambientColor);
    pointLight.setDiffuse(light.diffuseColor);
    pointLight.setSpecular(light.specularColor);
    pointLight.setAttenuation(light.attenuation);

    return addLight(pointLight);
}

void Scene::loadModels() {

}



