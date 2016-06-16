#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "PointLight.hpp"
#include "Constants.hpp"

struct ModelConfig {
    std::string name;
    std::string shader;
    std::string filename;
};

struct LightConfig {
    glm::vec3 position;

    glm::vec3 ambientColor{constants::LIGHTING_AMBIENT};
    glm::vec3 diffuseColor{constants::LIGHTING_DIFFUSE};
    glm::vec3 specularColor{constants::LIGHTING_SPECULAR};

    Attenuation attenuation = constants::LIGHTING_ATTENUATION;

};

struct Config {
    std::map<std::string, ModelConfig> models;

    std::map<int, LightConfig> lights;

    std::string shaderBasePath{"shaders/"};

    std::string modelsBasePath{"models/"};

    bool multiSamples = false;
    int numberOfSamples = 2;

    int width = 1280;
    int height = 720;
    bool startMaximized = true;

    float mouseSensitivity = constants::CAMERA_MOUSE_SENSITIVTY;
    float cameraSpeed = constants::CAMERA_SPEED;

    static Config loadConfig(const char *path);

    static Config loadConfig(const std::string &path);
};

