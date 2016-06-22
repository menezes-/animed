#define GLEW_STATIC

#include <GL/glew.h>
#include "../include/GUI.hpp"
#include "../include/Transform.hpp"
#include "../include/Utils/Utils.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <fmt/format.h>
#include <glm/gtx/string_cast.hpp>


inline void showHelpMarker(const char *desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(desc);
}

inline std::string matToText(const glm::mat4 &matrix) {

    std::string result{};

    for (int i = 0; i < 4; ++i) {
        std::string row{""};
        for (int j = 0; j < 4; ++j) {
            row += fmt::format("{: f} ", matrix[i][j]);
        }
        result += row + std::string{"\n"};
    }
    return result;
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
        static Transform *editedTransform = nullptr;
        static Transform tempTransform = Transform{};
        static glm::mat4 previousModelMatrix{};
        static int height = 700;
        ImGui::Begin("Geral", &showGeral, glm::vec2{800, height}, -1.0f);

        ImGui::BeginChild("models", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, height - 10), false,
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

            if (!scene.models.empty()) {
                if (ImGui::TreeNode("Instâncias")) {
                    static ImGuiTextFilter filterInstances;
                    filterInstances.Draw("Filtrar");
                    for (auto &instancia: scene.models) {
                        if (!filterInstances.PassFilter(instancia.objectName.c_str())) continue;
                        ImColor color = instancia.show ? ImColor{0, 255, 0} : ImColor{255, 0, 0};
                        ImGui::PushStyleColor(ImGuiCol_Text, color);
                        if (ImGui::TreeNode(fmt::format("{} {}", instancia.objectName, instancia.count).c_str())) {

                            ImGui::PushStyleColor(ImGuiCol_Text, ImColor{255, 255, 255});
                            const char *label = instancia.show ? "Esconder" : "Exibir";
                            if (ImGui::SmallButton(label)) {
                                instancia.show = !instancia.show;
                            }
                            ImGui::SameLine();
                            if (ImGui::SmallButton("Editar")) {
                                if (!editedTransform) {
                                    selectedInstance = &instancia;
                                    editedTransform = &instancia.transform;
                                    tempTransform = Transform{};
                                    previousModelMatrix = instancia.modelMatrix;
                                    instancia.showBorder = true;

                                }
                            }
                            ImGui::TreePop();
                            ImGui::PopStyleColor();
                        }
                        ImGui::PopStyleColor(1);
                    }
                    ImGui::TreePop();
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("config", ImVec2(0, height - 10), true);
        ImGui::Text("Configurações");
        if (editedTransform) {

            GLfloat escala = tempTransform.getScale().x;
            if (ImGui::DragFloat("Escala", &escala, 0.01f, .1f, 10.0f)) {
                tempTransform.setScale(glm::vec3{escala});
                tempTransform.apply(selectedInstance->modelMatrix);
            }
            ImGui::Separator();
            GLfloat RX = tempTransform.getRotation(Axis::X);
            GLfloat RY = tempTransform.getRotation(Axis::Y);
            GLfloat RZ = tempTransform.getRotation(Axis::Z);
            if (ImGui::SliderAngle("Rotção e. X", &RX)) {
                tempTransform.setRotation(RX, Axis::X);
                tempTransform.apply(selectedInstance->modelMatrix);
            }
            if (ImGui::SliderAngle("Rotção e. Y", &RY)) {
                tempTransform.setRotation(RY, Axis::Y);
                tempTransform.apply(selectedInstance->modelMatrix);
            }
            if (ImGui::SliderAngle("Rotção e. Z", &RZ)) {
                tempTransform.setRotation(RZ, Axis::Z);
                tempTransform.apply(selectedInstance->modelMatrix);
            }
            ImGui::Separator();
            glm::vec3 translate = tempTransform.getTranslate();
            if (ImGui::DragFloat3("Translate", (float *) &translate, 1.0f, -1000.0f, 1000.0f)) {
                tempTransform.setTranslate(translate);
                tempTransform.apply(selectedInstance->modelMatrix);
            }
            if (ImGui::Button("Aplicar", ImVec2(120, 0))) {

                *editedTransform = tempTransform;
                editedTransform = nullptr;
                tempTransform = Transform{};
                selectedInstance->modelMatrixOriginal = selectedInstance->modelMatrix;
                selectedInstance->showBorder = false;
                selectedInstance = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button("Resetar Valores")) {
                tempTransform = Transform{};
                selectedInstance->modelMatrix = previousModelMatrix;
            }
        } else {
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
            ImGui::Checkbox("Renderizar o chão", &scene.renderFloor);

            int numKeyframes = static_cast<int>(scene.numKeyframes);
            if (ImGui::InputInt("Número de KeyFrames", &numKeyframes, 1, 10)) {
                if (numKeyframes > 0) {
                    scene.numKeyframes = static_cast<std::size_t >(numKeyframes);
                    for (auto &model:scene.models) {
                        model.keyFrames.resize(scene.numKeyframes, Transform{});
                    }
                }
            }

            int numFrames = static_cast<int>(scene.numFrames);
            if (ImGui::InputInt("Número de frames\nintermediários", &numFrames, 1, 10)) {
                if (numFrames > 0) {
                    scene.numFrames = static_cast<std::size_t >(numFrames);
                }
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::End();
    }

    {
        if (mostrarDebug) {
            ImGui::Begin("Debug", &mostrarDebug, ImGuiWindowFlags_AlwaysAutoResize);

            if (ImGui::Button("copiar")) {
                ImGui::LogToClipboard(1);
            }
            ImGui::Text("Framerate %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            if (ImGui::CollapsingHeader("Câmera")) {
                ImGui::BulletText(
                        fmt::format("Posição: {}", glm::to_string(scene.getCamera().getPosition())).c_str());
                ImGui::BulletText(fmt::format("Front: {}", glm::to_string(scene.getCamera().getFront())).c_str());
                ImGui::BulletText(fmt::format("FOV: {}º", scene.getCamera().getFov()).c_str());
                ImGui::BulletText(fmt::format("Max FOV: {}º", scene.getCamera().getMaxFov()).c_str());
                ImGui::BulletText(fmt::format("Yaw: {}º", scene.getCamera().getYaw()).c_str());
                ImGui::BulletText(fmt::format("Pitch: {}º", scene.getCamera().getPitch()).c_str());
                ImGui::BulletText(fmt::format("Aspect: {}", scene.getCamera().getAspect()).c_str());
                ImGui::BulletText(fmt::format("Near Plane: {}", scene.getCamera().getNearPlane()).c_str());
                ImGui::BulletText(fmt::format("Far Plane: {}", scene.getCamera().getFarPlane()).c_str());
                ImGui::BulletText(
                        fmt::format("View Matrix:\n{}", matToText(scene.getCamera().getViewMatrix())).c_str());
                ImGui::BulletText(
                        fmt::format("Projection Matrix:\n{}",
                                    matToText(scene.getCamera().getProjectionMatrix())).c_str());

            }
            ImGui::End();
        }
    }


    if (!scene.models.empty()) {

        static Transform *editedTransform = nullptr;
        static Transform tempTransform = Transform{};
        static glm::mat4 previousModelMatrix{};

        ImGui::Begin("Animação");
        ImGui::Checkbox("Play", &scene.play);
        ImGui::SameLine();
        ImGui::Text(fmt::format("{} frame / {} keyframe", scene.currentFrame, scene.currentKeyFrame).c_str());
        if(ImGui::Button("Resetar")){
            scene.currentKeyFrame = 0;
            scene.currentFrame = 0;
        }
        ImGui::BeginChild("instances", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), false,
                          ImGuiWindowFlags_HorizontalScrollbar);

        if (ImGui::TreeNode("instancias_keyframe", "Instâncias")) {
            static ImGuiTextFilter filterInstances;
            filterInstances.Draw("Filtrar");
            for (auto &instancia: scene.models) {
                if (!filterInstances.PassFilter(instancia.objectName.c_str()) || !instancia.show) continue;
                if (ImGui::TreeNode(fmt::format("{} {}", instancia.objectName, instancia.count).c_str())) {
                    for (std::size_t j = 0; j < scene.numKeyframes; ++j) {
                        if (ImGui::Selectable(fmt::format("Keyframe {}", j + 1).c_str())) {
                            selectedInstanceAnimation = &instancia;
                            editedTransform = &instancia.keyFrames[j];
                            tempTransform = Transform{};
                            previousModelMatrix = instancia.modelMatrix;
                            instancia.showBorder = true;
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("configkeyframes", ImVec2(0, 300), true);
        ImGui::Text("KeyFrame");
        if (editedTransform) {

            GLfloat escala = tempTransform.getScale().x;
            if (ImGui::DragFloat("Escala", &escala, 0.01f, .1f, 10.0f)) {
                tempTransform.setScale(glm::vec3{escala});
                tempTransform.apply(selectedInstanceAnimation->modelMatrix);
            }
            ImGui::Separator();
            GLfloat RX = tempTransform.getRotation(Axis::X);
            GLfloat RY = tempTransform.getRotation(Axis::Y);
            GLfloat RZ = tempTransform.getRotation(Axis::Z);
            if (ImGui::SliderAngle("Rotção e. X", &RX)) {
                tempTransform.setRotation(RX, Axis::X);
                tempTransform.apply(selectedInstanceAnimation->modelMatrix);
            }
            if (ImGui::SliderAngle("Rotção e. Y", &RY)) {
                tempTransform.setRotation(RY, Axis::Y);
                tempTransform.apply(selectedInstanceAnimation->modelMatrix);
            }
            if (ImGui::SliderAngle("Rotção e. Z", &RZ)) {
                tempTransform.setRotation(RZ, Axis::Z);
                tempTransform.apply(selectedInstanceAnimation->modelMatrix);
            }
            ImGui::Separator();
            glm::vec3 translate = tempTransform.getTranslate();
            if (ImGui::DragFloat3("Translate", (float *) &translate, 1.0f, -1000.0f, 1000.0f)) {
                tempTransform.setTranslate(translate);
                tempTransform.apply(selectedInstanceAnimation->modelMatrix);
            }
            if (ImGui::Button("Aplicar", ImVec2(120, 0))) {

                *editedTransform = tempTransform;
                editedTransform = nullptr;
                tempTransform = Transform{};
                selectedInstanceAnimation->showBorder = false;
                selectedInstanceAnimation->modelMatrix = previousModelMatrix;
                selectedInstanceAnimation = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancelar")) {
                tempTransform = Transform{};
                selectedInstanceAnimation->modelMatrix = previousModelMatrix;
                selectedInstanceAnimation->showBorder = false;
                editedTransform = nullptr;
                selectedInstanceAnimation = nullptr;
            }
            if (editedTransform) {
                ImGui::Separator();
                ImGui::Text("Valores Atuais");
                ImGui::BulletText(fmt::format("Escala: {}", editedTransform->getScale().x).c_str());
                ImGui::BulletText(fmt::format("Rotação e. X: {}", editedTransform->getRotation(Axis::X)).c_str());
                ImGui::BulletText(fmt::format("Rotação e. Y: {}", editedTransform->getRotation(Axis::Y)).c_str());
                ImGui::BulletText(fmt::format("Rotação e. Z: {}", editedTransform->getRotation(Axis::Z)).c_str());
                ImGui::BulletText(
                        fmt::format("Translate: {}", glm::to_string(editedTransform->getTranslate())).c_str());
            }
        }


        ImGui::EndChild();
        ImGui::PopStyleVar(1);
        ImGui::End();

    }
}

GUI::GUI(Scene &scene, GLFWwindow *window) : scene(scene), window(window) {
    dragCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
}

