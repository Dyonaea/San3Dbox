#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Player{
    int id;
    glm::vec3 pos;
};


class World{
public:
    World();
    void run();
    void addPlayer();
    void remodePlayer();
    std::vector<struct Player*> players;
private:
int i = 0;

};