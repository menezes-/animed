#include "../include/Mesh.hpp"

GLuint Mesh::getVAO() const {
    return VAO;
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices,
           const std::vector<Texture> &textures)
        : vertices(vertices), indices(indices), textures(textures) {

    setup();

}

const std::vector<Vertex> &Mesh::getVertices() const {
    return vertices;
}

const std::vector<GLuint> &Mesh::getIndices() const {
    return indices;
}

const std::vector<Texture> &Mesh::getTextures() const {
    return textures;
}

void Mesh::setup() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);


    //teoricamente usar offsetof não é uma boa ideia, mas bem teoricamente. e é muita mão calcular os offsets eu mesmo
    // teoricamente: não daria certo pq offsetof não deveria funcionar com non-POD structs e tipos mais complexos
    // porém o struct é simples e a biblioteca GLM tem a linda propriedade que seus glm::vecN tem um layout compatível com o de um struct
    // ou seja seus membros estão lado a lado, de maneira que isso funciona.
    // ATENÇÃO: isso tudo é impirico, não encontrei em nenhum lugar  dizendo que o glm::vecN usa esse layout, porém fazer isso funciona.
    // Referencia caso offsetof não funcione mais: sizeof: 32 bytes (8 floats * 4 bytes (usar sizeof para pegar o tamanho do float pfvr))

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) 0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw(Shader &shader) const {

    std::array<GLuint, TextureType::MAX_TEXTURE_TYPE> textureIndex{};
    textureIndex.fill(1);

    for (std::size_t i = 0; i < textures.size(); ++i) {

        GLuint ui = static_cast<GLuint >(i);
        glActiveTexture(GL_TEXTURE0 + ui);

        //seta o uniform cujo nome é o padrão para este material com este indice (ver comentário em Mesh.hpp)
        Texture texture = textures[i];
        shader.setUniform1i(texture.getName(textureIndex[texture.type]++), ui);

        glBindTexture(GL_TEXTURE_2D, texture.id);

    }

    glBindVertexArray(VAO);
    GLsizei lsizei = static_cast<GLsizei >(indices.size());
    glDrawElements(GL_TRIANGLES, lsizei, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //como opengl é uma máquina de estados é boa prática deixar tudo como a gente recebeu
    for (std::size_t i = 0; i < textures.size(); ++i) {

        GLuint ui = static_cast<GLuint >(i);
        glActiveTexture(GL_TEXTURE0 + ui);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}


std::string Texture::getName(GLuint index) {
    static std::string base = "texture_";
    std::string typeStr;
    switch (type) {
        case DIFFUSE:
            typeStr = "diffuse";
            break;
        case SPECULAR:
            typeStr = "specular";
            break;
        case NORMAL:
            typeStr = "normal";
            break;
        case HEIGHT:
            typeStr = "height";
            break;
        case AMBIENT:
            typeStr = "ambient";
            break;
        case MAX_TEXTURE_TYPE:
            //pra dar erro mesmo, aprender a não ser retarado e usar coisa que não deve
            typeStr = "SEGURA_O_PAGODE";
            break;

    }
    return base + typeStr + std::to_string(index);
}
