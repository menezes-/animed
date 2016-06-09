#include "../include/Shader.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

std::string Shader::readFile(const std::string &filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        return (std::string{(std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()});
    }
    throw (strerror(errno));
}

Shader::Shader(const char *vertexPathp, const char *fragmentPathp) : vertexPath{vertexPathp},
                                                                     fragmentPath{fragmentPathp} {

    vertexCode = readFile(vertexPath);
    fragmentCode = readFile(fragmentPath);
    auto vertexShader = createShader(GL_VERTEX_SHADER, vertexCode);
    auto fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetShaderiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {

        std::vector<char> infoLog(100);
        glGetShaderInfoLog(program, 8096, nullptr, &infoLog[0]);
        throw std::runtime_error{std::string{std::string{"ERROR linkando o shader: "} + std::string{&infoLog[0]}}};

    }

    //a partir daqui é possível deletar os shaders
    //http://stackoverflow.com/questions/9113154/proper-way-to-delete-glsl-shader
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

}

GLuint Shader::createShader(GLenum type, const std::string &code) {

    const GLchar *code_ptr = code.c_str();

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &code_ptr, nullptr);
    glCompileShader(id);

    GLint success;

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {

        std::vector<char> infoLog(100);
        glGetShaderInfoLog(program, 8096, nullptr, &infoLog[0]);
        throw std::runtime_error{std::string{std::string{"ERROR Compilando shader: "} + std::string{&infoLog[0]}}};

    }

    return id;
}

void Shader::enable() {

    glUseProgram(program);

}

void Shader::disable() {

    glUseProgram(0);

}

void Shader::setMatrix4fv(const char *uniform_name, const GLfloat *value) {

    glUniformMatrix4fv(glGetUniformLocation(program, uniform_name), 1, GL_FALSE, value);

}

void Shader::setMatrix4fv(const std::string &uniform_name, const GLfloat *value) {
    setMatrix4fv(uniform_name.c_str(), value);
}

void Shader::setUniform3f(const char *uniform_name, const glm::vec3 &vec3) {

    glUniform3f(glGetUniformLocation(program, uniform_name), vec3.x, vec3.y, vec3.z);

}

void Shader::setUniform3f(const std::string &uniform_name, const glm::vec3 &value) {
    setUniform3f(uniform_name.c_str(), value);
}

void Shader::setUniform1i(const char *uniform_name, GLint i) {
    glUniform1i(glGetUniformLocation(program, uniform_name), i);

}

void Shader::setUniform1i(const std::string &uniform_name, GLint i) {
    setUniform1i(uniform_name.c_str(), i);
}

