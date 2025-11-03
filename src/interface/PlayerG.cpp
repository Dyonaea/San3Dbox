#include "San3Dbox/interface/PlayerG.hpp"
#include <glad/glad.h>

PlayerG::PlayerG(std::vector<Player>* players) {
    // --- Mesh du cube ---
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &instanceVBO); // <-- ici maintenant

    glBindVertexArray(VAO);

    // Données du cube
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attribut 0 = position du cube
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --- Données d'instances ---
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, players->size() * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    // Attribut 1 = position d’instance
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(1, 1); // 1 = change à chaque instance

    glBindVertexArray(0);
}

void PlayerG::update(std::vector<Player>* players) {
    std::vector<glm::vec3> positions;
    positions.reserve(players->size());

    for (auto& p : *players)
        positions.push_back(p.pos);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    static size_t currentSize = 0;
    size_t requiredSize = positions.size() * sizeof(glm::vec3);

    if (requiredSize > currentSize) {
        glBufferData(GL_ARRAY_BUFFER, requiredSize, positions.data(), GL_DYNAMIC_DRAW);
        currentSize = requiredSize;
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, requiredSize, positions.data());
    }
}

void PlayerG::render(std::vector<Player>* players) {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0, players->size());
}
