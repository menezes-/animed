#include "../include/Scene.hpp"
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>
#include "../include/Utils/Utils.hpp"


Scene::Scene(Config &config, Camera &camera, int width, int height) : config(config), camera(camera),
                                                                      shaderLoader{config.shaderBasePath},
                                                                      width{width}, height{height} {
    camera.setMouseSensitivity(config.mouseSensitivity);
    camera.setMovementSpeed(config.cameraSpeed);

    std::string lampModelPath = fmt::format("{}/{}/{}", config.modelsBasePath, "sphere", "sphere.obj");
    lampModel = std::make_unique<Model>(lampModelPath, textureLoader);
    LightConfig flashLightConfig{};
    flashLight = SpotLight::create(flashLightConfig.position, flashLightConfig.direction, 0);

    preLoadModels();

}

void Scene::draw() {


    flashLight->setPosition(camera.getPosition());
    flashLight->setDirection(camera.getFront());

    for (auto &model: models) {

        if (!model.show) continue;

        Shader &shader = model.shader;

        shader.enable();

        //atualiza os valores da camera
        applyUniforms(camera, shader);

        //seta o viewPos pras luzes funcionarem direito
        // viewPos é de viewer position
        shader.setUniform3f("viewPos", camera.getPosition());

        // ... But Tschaikovsky had the news
        // He said LET THERE BE LIGHT....

        for (const auto &light: lights) {
            applyUniforms(*light, shader);
        }
        // .. AND THERE WAS LIGHT
        flashLight->applyUniforms(shader);

        shader.setMatrix4fv("model", glm::value_ptr(model.modelMatrix));

        model.model.get().draw(shader);
    }
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

void Scene::newModelInstance(const std::string &objectName, glm::mat4 modelMatrix) {
    auto find = config.models.find(objectName);
    if (find == config.models.end()) {
        return;
    }
    auto mof = modelsObjs.find(objectName);
    if (mof == modelsObjs.end()) {
        return;
    } else {
        auto &pair = mof->second;
        Shader &shader = pair.second;
        Model &model = pair.first;
        ModelInstance instance{std::ref(model), std::ref(shader), modelMatrix};
        models.push_back(instance);
    }

}

void Scene::setRenderLights(bool renderLights) {
    this->renderLights = renderLights;
}

void Scene::toggleFlashLight() {
    renderFlashLight = !renderFlashLight;
    if (renderFlashLight) {
        flashLight->setSpecular(constants::LIGHTING_SPECULAR);
        flashLight->setDiffuse(constants::LIGHTING_DIFFUSE);
        flashLight->setAmbient(constants::LIGHTING_AMBIENT);
    } else {
        flashLight->setSpecular(glm::vec3{});
        flashLight->setDiffuse(glm::vec3{});
        flashLight->setAmbient(glm::vec3{});
    }
}

Camera &Scene::getCamera() const {
    return camera;
}

int Scene::getWidth() const {
    return width;
}

void Scene::setWidth(int width) {
    this->width = width;
}

int Scene::getHeight() const {
    return height;
}

void Scene::setHeight(int height) {
    this->height = height;
}

void Scene::preLoadModels() {
    for (const auto &mc: config.models) {
        auto modelConfig = mc.second;
        Shader &shader = shaderLoader.load(modelConfig.shader);
        std::string filepath = fmt::format("{}/{}/{}", config.modelsBasePath, modelConfig.name, modelConfig.filename);
        auto model1 = Model{filepath, textureLoader};
        modelsObjs.insert({mc.first, std::make_pair(model1, std::ref(shader))});
    }
}

ModelInstance::ModelInstance(const std::reference_wrapper<Model> &model, const std::reference_wrapper<Shader> &shader,
                             const glm::mat4 &modelMatrix) : model(model), shader(shader), modelMatrix(modelMatrix) {}
