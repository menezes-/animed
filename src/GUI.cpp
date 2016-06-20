#define GLEW_STATIC

#include <GL/glew.h>
#include "../include/GUI.hpp"
#include "../include/Scene.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


inline void showHelpMarker(const char *desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(desc);
}


void GUI::makeGUI() {

    ImGui_ImplGlfwGL3_NewFrame();
    {
        if (mostrarControles) {
            ImGui::Begin("Controles", &mostrarControles, glm::vec2{0, 0}, -1.0f, ImGuiWindowFlags_AlwaysAutoResize);
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
            ImGui::BulletText("Filtro de texto:\n"
                                      "   - Em branco: mostra todos os itens.\n"
                                      "   - \"xxx\": Linhas que contem \"xxx\".\n"
                                      "   - \"xxx,yyy\": Linhas que contem \"xxx\" e \"yyy\".\n"
                                      "   - \"-xxx\": Esconde linhas que contem \"xxx\".\n");
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
        static bool showGeral = true;
        ImGui::Begin("Geral", &showGeral, glm::vec2{800, 650}, -1.0f);

        ImGui::BeginChild("models", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300), false,
                          ImGuiWindowFlags_HorizontalScrollbar);

        if (ImGui::CollapsingHeader("Objetos")) {
            if (ImGui::TreeNode("Modelos")) {
                static ImGuiTextFilter filterModels;
                filterModels.Draw("Filtrar");
                for (const auto &model: scene.config.models) {
                    if (!filterModels.PassFilter(model.first.c_str())) continue;
                    if (ImGui::TreeNode(model.first.c_str())) {
                        if (ImGui::SmallButton("Criar Instância")) {
                            glfwSetCursor(window, dragCursor);
                            state = GUIState::MODEL_SELECTED;
                            nextInstance = &model.first;
                        }
                        ImGui::SameLine();
                        showHelpMarker(
                                "Vá com a câmera até o local desejado e, então, \n clique com o botão direito para criar uma nova instância do modelo naquela posição");
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("config", ImVec2(0, 300), true);
        ImGui::Text("Configurações");
        if (ImGui::ColorEdit3("Cor de fundo", (float *) &clear_color)) {
            // isso fica aqui para evitar ficar chamando a função clearColor
            // a cada frame.
            glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        }

        ImGui::Checkbox("Desenhar as luzes", &scene.renderLights);
        if (ImGui::Checkbox("Anti Aliasing", &scene.config.multiSamples)) {
            if (scene.config.multiSamples) {
                glEnable(GL_MULTISAMPLE);
            } else {
                glDisable(GL_MULTISAMPLE);
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::Text("Framerate %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

}

GUI::GUI(Scene &scene, GLFWwindow *window) : scene(scene), window(window) {
    dragCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
}

