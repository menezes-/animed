#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <array>
#include <glm/glm.hpp>
#include "../OpenGL/Shader.hpp"
#include "../Loaders/ShaderLoader.hpp"
#include "../Loaders/TextureLoader.hpp"
#include "../OpenGL/Camera.hpp"

class Floor {
public:
    Floor(ShaderLoader &shaderLoader, TextureLoader &textureLoader, Camera &camera);

    Shader &getShader() const;

    glm::mat4 getModelMatrix() const;

    void setModelMatrix(glm::mat4 modelMatrix);

    void draw();

    ~Floor();

private:

    GLuint VAO, VBO, textId;
    Shader &shader;
    Camera &camera;

    glm::mat4 modelMatrix{};


    std::array<GLfloat, 30> floorVertices{{
                                                  // Positions            // Texture Coords
                                                  5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
                                                  -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
                                                  -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

                                                  5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
                                                  -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
                                                  5.0f, -0.5f, -5.0f, 2.0f, 2.0f

                                          }};
};