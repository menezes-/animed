#include "../include/Scene.hpp"


void Scene::processInput(double deltaTime) {

}

Scene::Scene(int width, int height) : width(width), height(height), camera{glm::vec3{0.0f, 0.0f, 3.0f}, 45.0f,
                                                                           static_cast<float>(width) /
                                                                           static_cast<float>(height)} {
}