#include "../include/TextureLoader.hpp"
#include <SOIL2.h>
#include <iostream>
#include "../include/base64.hpp"

GLuint TextureLoader::loadFromPath(const std::string &path) {

    auto search = cache.find(path);

    if (search != cache.end()) {
        return search->second;
    }

    GLuint textId = SOIL_load_OGL_texture(
            path.c_str(),
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS

    );

    if (textId == 0) {
        std::cerr << "erro ao carregar textura " + path + ". Utilizando textura padrão" << '\n' << SOIL_last_result() << std::endl;
        return defaultTexture;
    }

    cache.insert({path, textId});

    return textId;
}


TextureLoader::TextureLoader() {
    auto decoded = base64_decode(defaultTextureData);
    defaultTexture = SOIL_load_OGL_texture_from_memory(
            decoded.data(),
            static_cast<int>(decoded.size()),
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS
    );

}

GLuint TextureLoader::load(const char *path) {
    return loadFromPath(std::string{path});
}

TextureLoader::~TextureLoader() {
    for (const auto &item: cache) {
        glDeleteTextures(1, &item.second);
    }

}





