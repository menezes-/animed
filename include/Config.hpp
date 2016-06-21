#pragma once
#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "Light/PointLight.hpp"
#include "Constants.hpp"

struct ModelConfig {
    std::string name;
    std::string shader;
    std::string filename;
};

struct LightConfig {
    glm::vec3 position{1,1,1};

    glm::vec3 direction{1,1,1};

    glm::vec3 ambientColor{constants::LIGHTING_AMBIENT};
    glm::vec3 diffuseColor{constants::LIGHTING_DIFFUSE};
    glm::vec3 specularColor{constants::LIGHTING_SPECULAR};

    /* Tabela com valores de atenuação
+===========+===========+=========+===========+
| Distance  | Constant  | Linear  | Quadratic |
+===========+===========+=========+===========+
|        7  | 1.0       | 0.7     | 1.8       |
+-----------+-----------+---------+-----------+
|       13  | 1.0       | 0.35    | 0.44      |
+-----------+-----------+---------+-----------+
|       20  | 1.0       | 0.22    | 0.20      |
+-----------+-----------+---------+-----------+
|       32  | 1.0       | 0.14    | 0.07      |
+-----------+-----------+---------+-----------+
|       50  | 1.0       | 0.09    | 0.032     |
+-----------+-----------+---------+-----------+
|       65  | 1.0       | 0.07    | 0.017     |
+-----------+-----------+---------+-----------+
|      100  | 1.0       | 0.045   | 0.0075    |
+-----------+-----------+---------+-----------+
|      160  | 1.0       | 0.027   | 0.0028    |
+-----------+-----------+---------+-----------+
|      200  | 1.0       | 0.022   | 0.0019    |
+-----------+-----------+---------+-----------+
|      325  | 1.0       | 0.014   | 0.0007    |
+-----------+-----------+---------+-----------+
|      600  | 1.0       | 0.007   | 0.0002    |
+-----------+-----------+---------+-----------+
|     3250  | 1.0       | 0.0014  | 0.000007  |
+-----------+-----------+---------+-----------+

     */

    Attenuation attenuation = constants::LIGHTING_ATTENUATION;

};

struct Config {
    std::map<std::string, ModelConfig> models;

    std::map<int, LightConfig> lights;

    std::string shaderBasePath{"shaders"};

    std::string modelsBasePath{"models"};

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

