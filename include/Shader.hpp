#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <unordered_map>
#include <glm/glm.hpp>


class Shader {

public:
    Shader(const char *vertexPath, const char *fragmentPath);

    void enable();

    void disable();

    void setMatrix4fv(const char *uniform_name, const GLfloat *value);

    void setUniform3f(const char *uniform_name, const glm::vec3 &);

    void setUniform1i(const char *uniform_name, GLint i);

    void setUniform1i(const std::string &uniform_name, GLint i);

    void setUniform1f(const char *uniform_name, GLfloat f);

    void setUniform1f(const std::string &uniform_name, GLfloat f);

    void setMatrix4fv(const std::string &uniform_name, const GLfloat *value);

    void setUniform3f(const std::string &uniform_name, const glm::vec3 &);

    GLuint getProgram() const;

    /**
     * Retorna a localização do uniform definido por uniform_name.
     * As localizações são guardadas em cache.
     */
    GLint getUniformLocation(const std::string &uniform_name);

    GLint getUniformLocation(const char *uniform_name);

private:
    GLuint program;

    std::string readFile(const std::string &filename);

    GLuint createShader(GLenum type, const std::string &code);

    std::string vertexPath;
    std::string fragmentPath;
    std::string fragmentCode;
    std::string vertexCode;

    /**
     * Faz um cache das localizações dos uniforms
     */
    std::unordered_map<std::string, GLint> locationCache;


};
