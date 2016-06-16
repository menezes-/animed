#pragma once
#include <glm/glm.hpp>

struct GUI {

    /**
     * mostra a ajuda dos controles
     */
    bool mostrarControles = true;

    bool drawLightsObjects = false;

    /**
     * cor de fundo
     */
    glm::vec4 clear_color{0.2f, 0.3f, 0.3f, 1.0f};

    /**
     * função chamada a cada interação do loop que monta os controles
     * e atualiza qualquer variavel (do próprio GUI struct) com os valores retornados
     * de interações com a interface.
     */
    void makeGUI();

};
