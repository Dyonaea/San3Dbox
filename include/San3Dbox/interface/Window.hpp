#pragma once
#include "San3Dbox/server/Server.hpp"
struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const char* title, SharedState *sharedState);
    ~Window();
    void run();

private:
    GLFWwindow* window;
    int width, height;
    const char* title;

    void processInput();
    void render();

    SharedState *sharedState;
};
