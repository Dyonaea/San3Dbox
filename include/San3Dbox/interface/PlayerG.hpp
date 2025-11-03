#pragma once
#include <glm/glm.hpp>
#include <San3Dbox/server/World.hpp>

class PlayerG{
    public:
        int id;
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
        PlayerG(std::vector<Player> *players);
        void update(std::vector<Player>* players);
        void render(std::vector<Player>* players);


    private:
        float vertices[27] = {
        -0.5f, -0.5f, -0.5f, // 0: back bottom left
            0.5f, -0.5f, -0.5f, // 1: back bottom right
            0.5f,  0.5f, -0.5f, // 2: back top right
        -0.5f,  0.5f, -0.5f, // 3: back top left
        -0.5f, -0.5f,  0.5f, // 4: front bottom left
            0.5f, -0.5f,  0.5f, // 5: front bottom right
            0.5f,  0.5f,  0.5f, // 6: front top right
        -0.5f,  0.5f,  0.5f  // 7: front top left
        };
        unsigned int indices[36] = {
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

    unsigned int VBO, VAO, EBO, instanceVBO;

};