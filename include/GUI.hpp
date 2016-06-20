#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Scene.hpp"


enum class GUIState : short {
    NORMAL,
    MODEL_SELECTED
};


struct GUI {

    /**
     * mostra a ajuda dos controles
     */
    bool mostrarControles = true;

    bool mostrarDebug = true;

    GUIState state = GUIState::NORMAL;

    Scene &scene;

    GLFWcursor *dragCursor;

    GLFWwindow *window;

    ModelInstance* selectedInstance = nullptr;

    const std::string *nextInstance = nullptr;

    /**
     * cor de fundo
     */
    glm::vec4 clear_color{0.2f, 0.3f, 0.3f, 1.0f};

    GUI(Scene &scene, GLFWwindow *window);

    /**
     * função chamada a cada interação do loop que monta os controles
     * e atualiza qualquer variavel (do próprio GUI struct) com os valores retornados
     * de interações com a interface.
     */
    void makeGUI();

};
