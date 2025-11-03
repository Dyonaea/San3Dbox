#include "San3Dbox/server/World.hpp"
#include <iostream>
#include <thread>
#include <chrono>

World::World(){
    std::cout << "world created" << std::endl;
    Player *p = new Player {0, glm::vec3(0.0f, 1.0f, 1.0f)};
    Player *p2 = new Player {1, glm::vec3(10.0f, 1.4f, 1.0f)};
    players.push_back(p);
    players.push_back(p2);
}
void World::run(){
    i++;
    if (i % 100 == 0){
        std::cout<< i << " | " << i/100 << " | " << players.size() << std::endl;
        players.push_back(new Player{i, glm::vec3(i/100, 0.0f, 0.0f)});
    }
    players[0]->pos.x += 0.002;

}
void World::addPlayer(){

}

void World::remodePlayer(){

}