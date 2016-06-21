#include <glm/gtc/type_ptr.hpp>
#include "../../include/Objects/Floor.hpp"
#include "../../include/Loaders/TextureLoader.hpp"

Floor::Floor(ShaderLoader &shaderLoader, TextureLoader &textureLoader, Camera &camera)
        : shader{shaderLoader.load("floor")}, camera{camera} {

    textId = textureLoader.load("textures/marble.jpg");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floorVertices.size(), &floorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glBindVertexArray(0);


    shader.enable();
    shader.setMatrix4fv("model", glm::value_ptr(this->modelMatrix));
    shader.disable();

}

Shader &Floor::getShader() const {
    return shader;
}

Floor::~Floor() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

}

glm::mat4 Floor::getModelMatrix() const {
    return modelMatrix;
}

void Floor::draw() {
    shader.enable();

    camera.applyUniforms(shader);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE30);
    shader.setUniform1i("texture1", 30);
    glBindTexture(GL_TEXTURE_2D, textId);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); // se isso não for feito da conflito com as outras texturas


}

void Floor::setModelMatrix(glm::mat4 modelMatrix) {
    this->modelMatrix = modelMatrix;
    shader.enable();
    shader.setMatrix4fv("model", glm::value_ptr(this->modelMatrix));
    shader.disable();
}
