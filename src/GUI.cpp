#define GLEW_STATIC

#include <GL/glew.h>
#include "../include/GUI.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


void GUI::makeGUI() {

    ImGui_ImplGlfwGL3_NewFrame();
    {
        if (mostrarControles) {
            ImGui::Begin("Controles", &mostrarControles, glm::vec2{0,0}, -1.0f, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::BulletText("Dê um duplo clique na barra de título para reduzir a janela.");
            ImGui::BulletText("Clique e arraste no canto inferior direito para redimensionar a janela.");
            ImGui::BulletText("Clique e arraste em um espaço vazio para mover a janela.");
            ImGui::BulletText("Roda do mouse para rolagem.");
            if (ImGui::GetIO().FontAllowUserScaling)
                ImGui::BulletText("CTRL + roda do mouse para dar zoom no conteúdo da janela.");
            ImGui::BulletText("TAB / SHIFT + TAB para trocar entre campos editáveis.");
            ImGui::BulletText("CTRL + Clique em um controle deslizante ou caixa de arrastar para inserir texto.");
            ImGui::BulletText(
                    "Ao editar texto:\n"
                            "   - Segure a tecla Shift ou use o mouse para selecionar o texto.\n"
                            "   - CTRL + Esquerda / Direita para andar uma palavra.\n"
                            "   - CTRL + A ou clique duas vezes para selecionar todos.\n"
                            "   - CTRL + X, CTRL + C, CTRL + V Área de Transferência.\n"
                            "   - CTRL + Z, CTRL + Y Desfazer / Refazer.\n"
                            "   - ESCAPE para reverter.\n"
                            "Você pode aplicar os operadores aritméticos +, *, / em valores numéricos.\n"
                            "Use +- para adicionar e diminuir.\n");
            ImGui::BulletText("Para mover a camera use as teclas W, A, S, D");
            ImGui::BulletText("Para destravar a camera aperte o botão direito do mouse.\n"
                                      "   - Com a cmaera destravada mova o mouse para olhar ao redor");
            ImGui::BulletText("Use o botão de scroll do mouse para dar zoom");
            ImGui::BulletText("Pressione F para ligar ou desligar a lanterna \n"
                                      "   (a lanterna não funciona com modelos que contem normal maping (bump maping))");
            ImGui::End();
        }
    }


    {
        ImGui::Begin("Geral");
        if(ImGui::ColorEdit3("Cor de fundo", (float *) &clear_color)){
            // isso fica aqui para evitar ficar chamando a função clearColor
            // a cada frame.
            glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        }

        ImGui::Checkbox("Desenhar as luzes", &drawLightsObjects);
        ImGui::Text("Framerate %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

}

