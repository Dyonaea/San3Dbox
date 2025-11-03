#pragma once
#include <vector>
#include "San3Dbox/interface/PlayerG.hpp"

class WorldRender{
public:
    WorldRender();
private:
    std::vector<PlayerG*> players;
};