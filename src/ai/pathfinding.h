//
// Created by Jack Armstrong on 1/13/20.
//

#ifndef GLMINECRAFT3_PATHFINDING_H
#define GLMINECRAFT3_PATHFINDING_H

#include <functional>
#include <glm/glm.hpp>
#include <vector>

namespace ai::path {

    struct path_options {
//        int max_nodes;
    };

    path_options default_opt();

    typedef std::function<bool(glm::vec3)> allowed_at_pos_func;
    typedef std::function<glm::vec3(allowed_at_pos_func, glm::vec3, glm::vec3,
                                    path_options)> pathfinder_func;//allowed_at,start,end,opt

    glm::vec3
    pathfind_astar(const allowed_at_pos_func &allowed_at, glm::vec3 start, glm::vec3 end, const path_options &opt);

}

#endif //GLMINECRAFT3_PATHFINDING_H
