#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class TextureLoader {
private:
    std::unordered_map<std::string, GLuint> cache;

    GLuint defaultTexture;

    const std::string defaultTextureData = std::string{"iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAAAAAA6fptVAAAAGHRFWHRTb2Z0d2Fy"
                                                               "ZQBwYWludC5uZXQgNC4wLjlsM35OAAAACklEQVQI12P4BwABAAD/wj7Y"
                                                               "wAAAAABJRU5ErkJggg=="};
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

    GLuint getDefaultTexture() const;

    virtual ~TextureLoader();


};
