#pragma once

#include "Camera.hpp"
#include <bitset>

class Scene {
public:


    Scene(int width, int height);

    std::bitset<1024> Keys{false};
    void processInput(double deltaTime);

private:
    int width;
    int height;
    Camera camera;

};