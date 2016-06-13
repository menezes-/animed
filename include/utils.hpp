#pragma once

#include <GL/glew.h>
#include <algorithm>
#include "PointLight.hpp"

namespace constants {
    //camera
    const GLfloat CAMERA_YAW = -90.0f;
    const GLfloat CAMERA_PITCH = 0.0f;
    const GLfloat CAMERA_SPEED = 3.0f;
    const GLfloat CAMERA_MOUSE_SENSITIVTY = 0.25f;
    const GLfloat CAMERA_FOV = 45.0f;
    const GLfloat CAMERA_MAX_FOV = 90.0f;
    const GLfloat CAMERA_ASPECT = 4.0f/3.0f;
    const GLfloat CAMERA_NEAR_PLANE= 0.1f;
    const GLfloat CAMERA_FAR_PLANE= 100.0f;

    //iluminação
    const glm::vec3 LIGHTING_AMBIENT{0.05f, 0.05f, 0.05f};
    const glm::vec3 LIGHTING_DIFFUSE{1.0f, 1.0f, 1.0f};
    const glm::vec3 LIGHTING_SPECULAR{1.0f, 1.0f, 1.0f};

    const Attenuation LIGHTING_ATTENUATION{1.0f,0.22f,0.20f};

}

/**
 * Prende o numero entre 'lower' e 'upper'
 * se for menor que lower, vira lower se maior que upper vira upper
 */
template <typename T>
T clamp(const T& lower, const T& n, const T& upper) {
    return std::max(lower, std::min(n, upper));
}
