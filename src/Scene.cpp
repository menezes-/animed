#include "../include/Scene.hpp"
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>
#include "../include/Utils/Utils.hpp"


Scene::Scene(Config &config, Camera &camera, int width, int height) : config(config), camera(camera),
                                                                      shaderLoader{config.shaderBasePath},
                                                                      width{width}, height{height},
                                                                      floor{shaderLoader, textureLoader, camera} {
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00);
    floor.draw();

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


        if (model.showBorder) {
            /*
             * eu vou evitar o máximo possível de overhead das bordas, então o código vai ficar "meio feio" com dois ifs
             * "separados" da mesma coisa. na prática eu poderia deixar essa parte inicial e só comentar o segundo passe,
             * mais abaixo
             */
            //desenha os objetos normalmente e enche o stencil buffer
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        shader.setMatrix4fv("model", glm::value_ptr(model.modelMatrix));
        model.model.get().draw(shader);

        if (model.showBorder) {
            //esse é o segundo passe, a gente desenha o objeto um pouco maior
            //pra usar o stencil buffer e desenhar as borda
            //nesse ponto o stencil buffer ta ""positivo"" (1) em tudo que é desenhado
            //então agora a gente "inverte", ou seja, tudo que for 1 não é desenhado
            //desenhando então só a diferença de tamanho entre os objetos (a gente escala o objeto um pouco no segundo passe)

            //PS: acho que tem uma outra função (glStencilFunc) que dava pra usar, ou usar uma mascara direto. não tenho cereteza
            // tem que testar
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST); // tem que desabilitar antes de desenhar o segundo passe se não da merda.

            //segundo ""passe"" propriamente dito
            stencilShader.enable();

            // tem que aplicar as mesmas transformaçoes pra deixar o bagulho no mesmo sistema de referência da camera
            applyUniforms(camera, stencilShader);

            glm::mat4 bigModel = model.modelMatrix;
            GLfloat scale = 1.05;
            bigModel = glm::scale(bigModel, glm::vec3(scale, scale, scale)); // escala só um pouco
            bigModel = glm::translate(bigModel, glm::vec3(0.0f, -.5f, 0.0f)); // escala só um pouco
            stencilShader.setMatrix4fv("model", glm::value_ptr(bigModel));
            model.model.get().draw(stencilShader);
            // terminou o segundo passe volta tudo ao normal
            glStencilMask(0xFF);
            glEnable(GL_DEPTH_TEST);
        }
    }
    /*
     * teoricamente apenar usar um glStencilMask(0x00); deveria "desabilitar" (não deveria encher o buffer sabe?)
     * o stencil pros objetos a seguir (e pra GUI também), porém, só usar o StencilMask não funciona direto
     * e da merda em toda a cena. Pode ser por causa da GUI ou pode ser que eu esteja fazendo algo errado.
     * Ou pode ser que o certo seja, realmente, desabi porem desabilitar GL_STENCIL_TEST, porém eu não encontrei nada sobre
     * o assunto. de qualquer forma sem o glDisable abaixo nada funciona.
     */


    //o chão e as lampadas não podem encher o stencil buffer
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
        auto counter = instanceCounter[objectName]++;
        auto &pair = mof->second;
        Shader &shader = pair.second;
        Model &model = pair.first;
        ModelInstance instance{std::ref(model), std::ref(shader), modelMatrix, objectName};
        instance.count = counter;
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
                             const glm::mat4 &modelMatrix, std::string objectName) : model(model), shader(shader),
                                                                                     modelMatrix(modelMatrix),
                                                                                     objectName{objectName} {}
