#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class TextureLoader {
private:
    std::unordered_map<std::string, GLuint> cache;

    GLuint defaultTexture;

    const std::string defaultTextureData = std::string{"iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAIAAACQd1PeAAAABGdBTUEAALGPC/xhBQAAAAlwSFlzAAALEgAACxIB0t1+/"
                                                               "AAAAAd0SU1FB9sEEBQWN66+rGcAAAAZdEVYdENvbW1lbnQAQ3JlYXRlZCB3aXRoIEdJTVBXgQ4XAAAAGHRFWH"
                                                               "RTb2Z0d2FyZQBwYWludC5uZXQgNC4wLjlsM35OAAAADElEQVQYV2P4b7UZAAQoAe0nEMWTAAAAAElFTkSuQmCC"};

public:

    TextureLoader();

    TextureLoader(TextureLoader &&) = default;

    TextureLoader &operator=(TextureLoader &&) = default;

    TextureLoader(const TextureLoader &) = delete;

    /**
     * Carrega uma textura de qualquer lugar
     */
    GLuint load(const char *path);

    GLuint loadFromPath(const std::string &path);

    virtual ~TextureLoader();


};
