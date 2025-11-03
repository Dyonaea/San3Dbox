#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "San3Dbox/interface/Window.hpp"
#include "San3Dbox/util/Shader.hpp"
#include "San3Dbox/util/Camera.hpp"
#include <iostream>
#include "San3Dbox/interface/PlayerG.hpp"

Window::Window(int width, int height, const char* title, SharedState *sharedState)
    : width(width), height(height), title(title), sharedState(sharedState) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::run() {
    // Ces objets ne changent pas
    PlayerG playerG(&sharedState->players);
    Camera camera(500, 500);
    Shader shader("./assets/shaders/default.vert", "./assets/shaders/default.frag");

    std::vector<Player> snapshot;

    while (!glfwWindowShouldClose(window)) {
        processInput();

        // Copier les joueurs en toute sécurité
        {
            std::lock_guard<std::mutex> lock(sharedState->mtx);
            snapshot = sharedState->players;
        }

        // Sécurité
        if (snapshot.empty())
            continue;

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        camera.inputs(window);
        camera.Matrix(80.0f, 0.1f, 100.0f, &shader, "camMatrix");

        // Mise à jour et rendu
        playerG.update(&snapshot);
        playerG.render(&snapshot);
        std::cout << snapshot.size() << std::endl;
        // Debug
        if (snapshot.size() >= 2) {
            // std::cout << snapshot[0].pos.x << " " << snapshot[1].pos.x << std::endl;
        }

        shader.setFloat("gap", snapshot[0].pos.x);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void Window::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Window::render() {
    



    // Setup VAO / VBO / EBO




}
