#include <fmt/format.h>
#include "../include/Scene.hpp"
#include <glm/gtc/type_ptr.hpp>


Scene::Scene(const Config &config, Camera &camera) : config(config), camera(camera),
                                                     shaderLoader{config.shaderBasePath} {
    camera.setMouseSensitivity(config.mouseSensitivity);
    camera.setMovementSpeed(config.cameraSpeed);

    std::string lampModelPath = fmt::format("{}/{}/{}", config.modelsBasePath, "sphere", "sphere.obj");
    lampModel = std::unique_ptr<Model>(new Model{lampModelPath, textureLoader});

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
            applyUniforms(light, shader);
        }
        // .. AND THERE WAS LIGHT

        Model &modelObj = model.first;

        //Matriz de transformação original do modelo
        //todas as transformações deve ser feitas a partir desta matriz
        glm::mat4 modelMatrix = modelObj.getModelMatrix();

        shader.setMatrix4fv("model", glm::value_ptr(modelMatrix));

        modelObj.draw(shader);

        if (renderLights) {

            Shader &lampShader = shaderLoader.load("lamp");
            lampShader.enable();

            applyUniforms(camera, lampShader);
            for (auto &light: lights) {
                glm::mat4 lampModel{};
                lampModel = glm::scale(lampModel, glm::vec3(0.1f, 0.1f, 0.1f));
                lampModel = glm::translate(lampModel, light.getPosition());
                lampShader.setMatrix4fv("model", glm::value_ptr(lampModel));
                this->lampModel->draw(lampShader);
            }


        }
    }
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
    models.emplace_back(std::make_pair(Model{filepath, textureLoader, transform}, std::ref(shader)));
}


void Scene::setRenderLights(bool renderLights) {
    this->renderLights = renderLights;
}

