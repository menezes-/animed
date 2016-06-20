#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "../OpenGL/Shader.hpp"


/**
 * Convenção de nome de texturas:
 *
 * os meshs dos objetos podem ter várias texturas de diferentes tipo (tipos suportados definidos no enum abaixo)
 * portando vamos usar o seguinte padrão (que na verdade é bem padrão e eu copiei de outros lugares, mas ok):
 * texture_$tipo$n $n é o indicie da textura (GL_TEXTURE0 + $n) e tipo é o tipo mesmo (diffuse, specular, etc)
 * de maneira que no shader vc define, por exemplo:
 * uniform sampler2D texture_diffuse1;
 * uniform sampler2D texture_specular1;
 *
 */

enum TextureType : short {

    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    AMBIENT,
    MAX_TEXTURE_TYPE

};

struct Texture {
    GLuint id;

    TextureType type;

    std::string getName(GLuint index);
};

struct Vertex {
    // Position
    glm::vec3 position;

    // Normal
    glm::vec3 normal;

    // TexCoords
    glm::vec2 texCoords;

    // Tangent
    glm::vec3 tangent;

    // Bitangent
    glm::vec3 bitangent;
};

class Mesh {
public:
    GLuint getVAO() const;


    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures);


    const std::vector<Vertex> &getVertices() const;

    const std::vector<GLuint> &getIndices() const;

    const std::vector<Texture> &getTextures() const;

    void draw(Shader &shader) const;

    // a classe não é copiavel pois ela contém referencias a resources do opengl que são destruidos no destrutor
    // portanto se fosse possível gerar copias do objetos ocorreria um double free dos resources do opengl
    // (dado que cada copia tentaria deletar os seus respectivos VAOs, VBOs e EBOs que apontam para os mesmos recursos no opengl)

    //virtual ~Mesh();


private:
    GLuint VAO, VBO, EBO;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    void setup();


};
