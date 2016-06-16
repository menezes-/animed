#include "../include/ShaderLoader.hpp"
#include <fmt/format.h>


ShaderLoader::ShaderLoader(std::string basePath) : basePath{basePath} {

}

Shader &ShaderLoader::load(const std::string &name) {
    auto find = cache.find(name);
    if (find == cache.end()) {
        std::string path_vs = fmt::format("{}/{}.vs.glsl", basePath, name);
        std::string path_fs = fmt::format("{}/{}.fs.glsl", basePath, name);
        auto ins = cache.insert({name, Shader{path_vs, path_fs}});
        return ins.first->second;
    }

    return find->second;
}

Shader &ShaderLoader::load(const char *name) {
    return load(std::string{name});
}





