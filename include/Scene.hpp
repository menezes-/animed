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
    Scene(const Config &config, Camera &camera);

    void draw();

    void newModelInstance(const std::string &objectName);

    void newModelInstance(const std::string &objectName, const Transform &transform);

    void setRenderLights(bool renderLights);

/**
 * Esse método é tipo uma factory que cria luz para as cenas.
 * Não necessariamente o melhor desgin pra isso, mas sla tb.
 * @param type tipo da luz, ex: POINT, SPOT.
 * @param lightConfig parametros básicos de configuração de luz, ex: atenuação, cor da luz ambiente, especular, difusa, etc
 * @return pointer para a luz para maiores configurações
 */
    Light *addLight(LightType type, const LightConfig &lightConfig);

    void toggleFlashLight(bool onOff);

private:
    const Config &config;
    Camera &camera;
    ShaderLoader shaderLoader;
    std::unique_ptr<Model> lampModel;

    bool renderLights{false};

    TextureLoader textureLoader{};

    std::unique_ptr<SpotLight> flashLight;

    //mapa que mantém os ids das luzes para cada tipo de luz
    // isso é util para arrays de luzes onde o id corresponde a posição do array no shader
    LightTypeHash<LightType, int> lightsCounter{};

    std::vector<std::unique_ptr<Light>> lights;

    std::vector<std::pair<Model, std::reference_wrapper<Shader> > > models;

    template<class T>
    void applyUniforms(const T &obj, Shader &shader) {
        obj.applyUniforms(shader);
    }
};
