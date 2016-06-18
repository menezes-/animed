#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>
#include "../include/Utils.hpp"
#include "../include/Scene.hpp"
#include "../include/SpotLight.hpp"
#include "../include/PointLight.hpp"


Scene::Scene(const Config &config, Camera &camera) : config(config), camera(camera),
                                                     shaderLoader{config.shaderBasePath} {
    camera.setMouseSensitivity(config.mouseSensitivity);
    camera.setMovementSpeed(config.cameraSpeed);

    std::string lampModelPath = fmt::format("{}/{}/{}", config.modelsBasePath, "sphere", "sphere.obj");
    lampModel = std::make_unique<Model>(lampModelPath, textureLoader);

}

void Scene::draw() {

    for (const auto &model: models) {
        Shader &shader = model.second;

        shader.enable();

        //atualiza os valores da camera
        applyUniforms(camera, shader);

        //seta o viewPos pras luzes funcionarem direito
        shader.setUniform3f("viewPos", camera.getPosition());

        // ... But Tschaikovsky had the news
        // He said LET THERE BE LIGHT....

        for (const auto &light: lights) {
            applyUniforms(*light, shader);
        }
        // .. AND THERE WAS LIGHT

        const Model &modelObj = model.first;

        //Matriz de transformação original do modelo
        //todas as transformações deve ser feitas a partir desta matriz
        glm::mat4 modelMatrix = modelObj.getModelMatrix();

        shader.setMatrix4fv("model", glm::value_ptr(modelMatrix));

        modelObj.draw(shader);

        if (renderLights && lights.size() > 0) {

            Shader &lampShader = shaderLoader.load("lamp");
            lampShader.enable();

            applyUniforms(camera, lampShader);
            for (auto &light: lights) {
                glm::mat4 lampModel{};
                lampModel = glm::scale(lampModel, glm::vec3(0.1f, 0.1f, 0.1f));
                lampModel = glm::translate(lampModel, light->getPosition());
                lampShader.setMatrix4fv("model", glm::value_ptr(lampModel));
                this->lampModel->draw(lampShader);
            }


        }
    }
}


Light *Scene::addLight(LightType type, const LightConfig &lightConfig) {

    int id = lightsCounter[type]++;

    switch (type) {

        case LightType::SPOT:
            lights.emplace_back(SpotLight::create(lightConfig.position, lightConfig.direction, id));
            break;
        case LightType::POINT:
            lights.emplace_back(PointLight::create(lightConfig.position, id));
            break;
    }
    Light *light = lights.back().get();
    light->setAmbient(lightConfig.ambientColor);
    light->setDiffuse(lightConfig.diffuseColor);
    light->setSpecular(lightConfig.specularColor);
    light->setAttenuation(lightConfig.attenuation);

    return light;

}

void Scene::newModelInstance(const std::string &objectName) {
    Transform defaultTransform{};
    newModelInstance(objectName, defaultTransform);
}

void Scene::newModelInstance(const std::string &objectName, const Transform &transform) {
    auto find = config.models.find(objectName);
    if (find == config.models.end()) {
        return;
    }

    auto modelConfig = find->second;

    Shader &shader = shaderLoader.load(modelConfig.shader);
    std::string filepath = fmt::format("{}/{}/{}", config.modelsBasePath, modelConfig.name, modelConfig.filename);
    auto model = Model{filepath, textureLoader, transform};
    models.push_back(std::make_pair(model, std::ref(shader)));
}


void Scene::setRenderLights(bool renderLights) {
    this->renderLights = renderLights;
}
