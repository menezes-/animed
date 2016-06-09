#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "include/Shader.hpp"
#include "include/Model.hpp"
#include "include/TextureLoader.hpp"
#include "include/Camera.hpp"


static GLFWwindow *setupGraphics(unsigned int screenWidth, unsigned int screenHeight) {
    if (!glfwInit()) {
        throw std::runtime_error{"Nao foi possivel inicializar a biblioteca GLFW 3"};
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(screenWidth, screenHeight, "AnimaEd", nullptr, nullptr);

    if (window == nullptr) {
        throw std::runtime_error{"Nao foi possivel criar a janela!"};

    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        throw std::runtime_error{"Nao foi possivel inicializar o glew! "};
    }
    glEnable(GL_DEPTH_TEST);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSwapInterval(1);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    return window;


}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void doMovement();

bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLFWwindow *window;

int main() {
    window = setupGraphics(1024, 1024);

    TextureLoader textureLoader = TextureLoader();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    Camera camera{glm::vec3{0.0f, 0.0f, 3.0f}, 45.0f,
                  static_cast<float>(width) /
                  static_cast<float>(height)};

    glfwSetWindowUserPointer(window, &camera);

    Shader shader{"nano.vert", "nano.frag"};

    Model modelObj{std::string{"models/nanosuit/nanosuit.obj"}, textureLoader, shader};

    while (!glfwWindowShouldClose(window)) {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        doMovement();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.enable();
        shader.setMatrix4fv("projection", glm::value_ptr(camera.getProjectionMatrix()));
        shader.setMatrix4fv("view", glm::value_ptr(camera.getViewMatrix()));

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f,
                                                0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));    // It's a bit too big for our scene, so scale it down
        shader.setMatrix4fv("model", glm::value_ptr(model));
        modelObj.draw();
        glfwSwapBuffers(window);

    }
    glfwTerminate();
    return EXIT_SUCCESS;

}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void doMovement() {
    auto camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    if (keys[GLFW_KEY_W])
        camera->move(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera->move(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera->move(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera->move(RIGHT, deltaTime);
}