#pragma once

#include "Config.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Constants.hpp"
#include "TextureLoader.hpp"
#include "Model.hpp"
#include "ShaderLoader.hpp"
#include "Light.hpp"
#include "SpotLight.hpp"
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <memory>

/*
 * O struct abaixo representa uma instancia de um obj:
 * - um pointeiro para o Model do objeto
 * - a model matrix do objeto
 *   (todas as transformações serao feitas em cima dessa matrix )
 * - uma lista de transformações. Essas transformações são os keyframes do objeto e usadas na animação
 * - um boleano que controla se a instancia é desenhada ou não
 */

struct ModelInstance {

    std::reference_wrapper<Model> model;
    std::reference_wrapper<Shader> shader;
    glm::mat4 modelMatrix{};
    std::vector<Transform> keyFrames{};

    bool show = true;

    ModelInstance(const std::reference_wrapper<Model> &model, const std::reference_wrapper<Shader> &shader,
                  const glm::mat4 &modelMatrix);

};

/*
 *c++11 não provê especializações do std::hash para enum class, então eu uso o functor abaixo para
 * criar um uma especialização apra o meu enum class.
 * Para deixar as coisas mais interessantes o using abaixo decide se o map deve usar o meu functor ou o std::hash
 * para criar o map, de maneira que eu posso usar sem passar o parametro de hash para o map
 * melhor explicado aqui:http://stackoverflow.com/a/24847480/482238
 */

// functor que da a especialização para o meu enum
struct EnumClassHash {
    template<typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

//sugar que facilita a vida
template<typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template<typename Key, typename T>
using LightTypeHash = std::unordered_map<Key, T, HashType<Key>>;

class Scene {
public:
    Scene(const Config &config, Camera &camera, int width, int height);

    void draw();

    void newModelInstance(const std::string &objectName, glm::mat4 modelMatrix);

    void setRenderLights(bool renderLights);

/**
 * Esse método é tipo uma factory que cria luz para as cenas.
 * Não necessariamente o melhor desgin pra isso, mas sla tb.
 * @param type tipo da luz, ex: POINT, SPOT.
 * @param lightConfig parametros básicos de configuração de luz, ex: atenuação, cor da luz ambiente, especular, difusa, etc
 * @return pointer para a luz para maiores configurações
 */
    Light *addLight(LightType type, const LightConfig &lightConfig);

    void toggleFlashLight();

    Camera &getCamera() const;

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

private:
    const Config &config;
    Camera &camera;
    ShaderLoader shaderLoader;
    int width, height;
    std::unique_ptr<Model> lampModel;

    bool renderLights{false};

    bool renderFlashLight{true};

    TextureLoader textureLoader{};

    std::unique_ptr<SpotLight> flashLight;

    //mapa que mantém os ids das luzes para cada tipo de luz
    // isso é util para arrays de luzes onde o id corresponde a posição do array no shader
    LightTypeHash<LightType, int> lightsCounter{};

    std::vector<std::unique_ptr<Light>> lights;

    std::vector<ModelInstance> models;

    std::unordered_map<std::string, std::pair<Model, std::reference_wrapper<Shader>>> modelsObjs{};

    void preLoadModels();

    template<class T>
    void applyUniforms(const T &obj, Shader &shader) {
        obj.applyUniforms(shader);
    }

};
