#include "../include/Config.hpp"
#include "../include/Utils.hpp"
#include <ini.h>
#include <fmt/format.h>
#include <algorithm>
#include <string>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


inline glm::vec3 parseVec3(const std::string &value) {
    std::vector<std::string> tokens{};

    tokenize(value, tokens, std::string{" "}, true);

    if (tokens.size() != 3) {
        return glm::vec3{0.0f, 0.0f, 0.0f};
    }

    float x, y, z;

    toFloat(tokens.at(0), x);
    toFloat(tokens.at(1), y);
    toFloat(tokens.at(2), z);

    return glm::vec3{x, z, y};

}

inline int handler(void *cfg, const char *section, const char *name, const char *value) {

    auto config = static_cast<Config *>(cfg);
    auto string_name = std::string{name};
    auto string_value = std::string{value};
    auto string_section = std::string{section};
    toLower(string_name);
    toLower(string_value);
    toLower(string_section);

    int pvalue;
    float fvalue;
    if (startsWith(string_section, "app") || string_section.empty()) {
        if (string_name == "multisamples") {
            config->multiSamples = toBool(string_value);
        } else if (string_name == "numberofsamples") {
            if (toInt(string_value, pvalue)) {
                config->numberOfSamples = pvalue;
            }
        } else if (string_name == "width") {
            if (toInt(string_value, pvalue)) {
                config->width = pvalue;
            }
        } else if (string_name == "height") {
            if (toInt(string_value, pvalue)) {
                config->height = pvalue;
            }
        } else if (string_name == "startmaximized") {
            config->startMaximized = toBool(string_value);
        } else if (string_name == "mousesensitivity") {
            if (toFloat(string_value, fvalue)) {
                config->mouseSensitivity = fvalue;
            }
        } else if (string_name == "cameraspeed") {
            if (toFloat(string_value, fvalue)) {
                config->cameraSpeed = fvalue;
            }
        } else {
            return 0;
        }
    } else if (startsWith(string_section, "model_")) {
        std::string modelName = string_section.substr(5, string_section.length());

        ModelConfig &model = config->models[modelName];

        if (string_name == "name") {
            model.name = string_value;
        } else if (string_name == "shader") {
            model.shader = string_value;
        } else if (string_name == "filename") {
            model.shader = string_value;
        } else {
            return 0;
        }

    } else if (startsWith(string_section, "light_")) {
        std::string lightIndex = string_section.substr(5, string_section.length());

        int lii;
        if (toInt(lightIndex, lii)) {
            if (lii > constants::LIGHTING_MAX_LIGHTS) {
                return 1;
            }
            LightConfig &lc = config->lights[lii];

            if (string_name == "ambientcolor") {
                lc.ambientColor = parseVec3(string_value);
            } else if (string_name == "diffusecolor") {
                lc.diffuseColor = parseVec3(string_value);
            } else if (string_name == "specularcolor") {
                lc.specularColor = parseVec3(string_value);
            } else if (string_name == "position") {
                lc.position = parseVec3(string_value);
            } else if (string_name == "constant") {
                if (toFloat(string_value, fvalue)) {
                    lc.attenuation.constant = fvalue;
                }
            } else if (string_name == "linear") {
                if (toFloat(string_value, fvalue)) {
                    lc.attenuation.linear = fvalue;
                }
            } else if (string_name == "quadratic") {
                if (toFloat(string_value, fvalue)) {
                    lc.attenuation.quadratic = fvalue;
                }
            } else {
                return 0;
            }

        } else {
            return 0;
        }
    }
    else {
        return 0;
    }
    return 1;

}

Config Config::loadConfig(const char *path) {
    auto cfg = Config();
    if (ini_parse(path, handler, &cfg) < 0) {
        std::runtime_error{
                fmt::format("WARNING::CONFIGURATION: Nao foi possivel carregar o arquivo de configuração.  {} ", path)};
    }
    return cfg;
}

Config Config::loadConfig(const std::string &path) {
    return loadConfig(path.c_str());
}


