#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "include/Shader.hpp"
#include "include/Model.hpp"
#include "include/TextureLoader.hpp"
#include "include/Camera.hpp"
#include "include/PointLight.hpp"
#include "include/GUI.hpp"
#include "include/Config.hpp"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void window_resize_callback(GLFWwindow *window, int width, int height);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void doMovement();

static GLFWwindow *setupGraphics(const Config &config) {
    if (!glfwInit()) {
        throw std::runtime_error{"Nao foi possivel inicializar a biblioteca GLFW 3"};
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    if (config.multiSamples) {
        glfwWindowHint(GLFW_SAMPLES, config.numberOfSamples);
    }

    int screenWidth, screenHeight;
    if (config.startMaximized) {
        screenWidth = mode->width;
        screenHeight = mode->height;

    } else {
        screenWidth = config.width;
        screenHeight = config.height;
    }
    auto window = glfwCreateWindow(screenWidth, screenHeight, "AnimaEd", nullptr, nullptr);

    if (window == nullptr) {
        throw std::runtime_error{"Nao foi possivel criar a janela!"};

    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, window_resize_callback);
    glfwSetCharCallback(window, ImGui_ImplGlfwGL3_CharCallback);

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        throw std::runtime_error{"Nao foi possivel inicializar o glew! "};
    }

    glEnable(GL_DEPTH_TEST);
    if (config.multiSamples) {
        glEnable(GL_MULTISAMPLE);
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glfwSwapInterval(1);
    return window;


}

void doMovement();

bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool mouseLock = true;
GLFWwindow *window;

int main() {
    Config config = Config::loadConfig("config.ini");

    window = setupGraphics(config);

    TextureLoader textureLoader = TextureLoader();
    GUI gui;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    Camera camera{glm::vec3{0.0f, 0.0f, 3.0f}};
    //camera.setPitch(-30.0f);
    camera.setMaxFov(90.0f);

    glfwSetWindowUserPointer(window, &camera);

    Shader shader{"shaders/model.vs.glsl", "shaders/model.fs.glsl"};

    Lights lights;

    PointLight light1{glm::vec3{2.3f, -1.6f, -3.0f}};
    light1.setLightContainer("pointLights");
    light1.setId(0);

    PointLight light2{glm::vec3{-1.7f, 0.9f, 1.0f}};
    light2.setLightContainer("pointLights");
    light2.setId(1);

    lights.lights.push_back(light1);
    lights.lights.push_back(light2);

    Model modelObj{std::string{"models/nanosuit/nanosuit.obj"}, textureLoader};

    ImGui_ImplGlfwGL3_Init(window, false);

    while (!glfwWindowShouldClose(window)) {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        gui.makeGUI();

        doMovement();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.enable();
        shader.setMatrix4fv("projection", glm::value_ptr(camera.getProjectionMatrix()));
        shader.setMatrix4fv("view", glm::value_ptr(camera.getViewMatrix()));
        shader.setUniform3f("viewPos", camera.getPosition());
        lights.draw(shader);

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        //model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMatrix4fv("model", glm::value_ptr(model));
        modelObj.draw(shader);
        ImGui::Render();
        glfwSwapBuffers(window);

    }
    glfwTerminate();
    return EXIT_SUCCESS;

}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mode);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
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

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (!mouseLock) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

        lastX = xpos;
        lastY = ypos;
        auto camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));

        camera->processMouseMovement(xoffset, yoffset);
    }
}


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);
    auto camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    camera->zoom(yoffset);
}

void window_resize_callback(GLFWwindow *window, int width, int height) {

    int nwidth, nheight;
    glfwGetFramebufferSize(window, &nwidth, &nheight);
    glViewport(0, 0, nwidth, nheight);

    auto camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));

    camera->setAspect(static_cast<float>(width) / static_cast<float>(height));

}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouseLock = !mouseLock;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        std::cout << x << ' ' << y << std::endl;

    }
}