#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "San3Dbox/interface/Window.hpp"
#include "San3Dbox/util/Shader.hpp"
#include "San3Dbox/util/Camera.hpp"
#include <iostream>

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
    std::vector<Player> snapshot;
    Camera *camera = new Camera(500, 500);

    Shader *shader = new Shader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
    while (!glfwWindowShouldClose(window)) {
        processInput();
        shader->use();
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        camera->inputs(window);
        camera->Matrix(80.0f, 0.1f, 100.0f, shader, "camMatrix");
        
        std::lock_guard<std::mutex> lock(sharedState->mtx);
        snapshot = sharedState->players;
        std::cout << snapshot[0].pos.x << std::endl;
        std::cout << snapshot[1].pos.x << std::endl;
        shader->setFloat("gap", snapshot[0].pos.x);
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
    float vertices[] = {
        // positions
    -0.5f, -0.5f, -0.5f, // 0: back bottom left
        0.5f, -0.5f, -0.5f, // 1: back bottom right
        0.5f,  0.5f, -0.5f, // 2: back top right
    -0.5f,  0.5f, -0.5f, // 3: back top left
    -0.5f, -0.5f,  0.5f, // 4: front bottom left
        0.5f, -0.5f,  0.5f, // 5: front bottom right
        0.5f,  0.5f,  0.5f, // 6: front top right
    -0.5f,  0.5f,  0.5f  // 7: front top left
    };

    unsigned int indices[] = {
        // back face
        0, 1, 2,
        2, 3, 0,
        // front face
        4, 5, 6,
        6, 7, 4,
        // left face
        4, 0, 3,
        3, 7, 4,
        // right face
        1, 5, 6,
        6, 2, 1,
        // bottom face
        4, 5, 1,
        1, 0, 4,
        // top face
        3, 2, 6,
        6, 7, 3
    };

    // Setup VAO / VBO / EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}
