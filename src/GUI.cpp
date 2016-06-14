#include "../include/GUI.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

void GUI::makeGUI() {

    ImGui_ImplGlfwGL3_NewFrame();
    {
        if (mostrarControles) {
            ImGui::Begin("Controles", &mostrarControles);
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
                            "- Segure a tecla Shift ou use o mouse para selecionar o texto.\n"
                            "- CTRL + Esquerda / Direita para andar uma palavra.\n"
                            "- CTRL + A ou clique duas vezes para selecionar todos.\n"
                            "- CTRL + X, CTRL + C, CTRL + V Área de Transferência.\n"
                            "- CTRL + Z, CTRL + Y Desfazer / Refazer.\n"
                            "- ESCAPE para reverter.\n"
                            "Você pode aplicar os operadores aritméticos +, *, / em valores numéricos.\n"
                            "Use +- para adicionar e diminuir.\n");
            ImGui::End();
        }
    }


    {
        ImGui::Begin("Geral");
        ImGui::ColorEdit3("Cor de fundo", (float *) &clear_color);
        ImGui::End();
    }

}

