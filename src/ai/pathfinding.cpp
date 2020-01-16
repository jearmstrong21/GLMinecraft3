//
// Created by Jack Armstrong on 1/13/20.
//

#include "pathfinding.h"
#include <utility>
#include <algorithm>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <unordered_map>
#include <unordered_set>

namespace ai::path {

    path_options default_opt(){
        path_options opt{};
//        opt.max_nodes=1000;
        return opt;
    }

    glm::vec3 pathfind_astar(const allowed_at_pos_func& allowed_at,glm::vec3 _start,glm::vec3 _end,const path_options& opt){
        glm::ivec3 start{_start};
        glm::ivec3 end{_end};

        std::unordered_map<glm::ivec3,glm::ivec3>previous;
        std::unordered_map<glm::ivec3,int>pathlen;
        std::unordered_set<glm::ivec3>open;
        std::unordered_set<glm::ivec3>closed;

        open.insert(start);
        return glm::vec3{0};
//        while(open.size()>0){}
    }

}