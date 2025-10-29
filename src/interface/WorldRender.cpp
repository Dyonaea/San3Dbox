#include "San3Dbox/interface/WorldRender.hpp"
#include "San3Dbox/util/Shader.hpp"

WorldRender::WorldRender(){
    Shader("./assets/shaders/default.vert", "./assets/shaders/default.frag");
}