#pragma once

#include <string>
#include <unordered_map>
#include "Shader.hpp"

class ShaderLoader {
public:

    ShaderLoader(const std::string& basePath);

    Shader& load(const std::string &name);
    Shader& load(const char *name);

private:
    std::string basePath;

    std::unordered_map<std::string, Shader> cache;



};
