//
// Created by Jack Armstrong on 1/13/20.
//

#ifndef GLMINECRAFT3_PATHFINDING_H
#define GLMINECRAFT3_PATHFINDING_H

#include <functional>
#include <glm/glm.hpp>
#include <vector>

namespace ai::path {

    struct path_options{
        int max_search_nodes;//raw max num of nodes to look at, approx search range = cbrt(max_search_nodes)
        float ok_distance;//terminate if within ok_distance of end; setting to zero almost always will not return valid result
        std::vector<glm::ivec3> cardinal_directions;//left/right/front/back, TODO: not ivec3
        bool can_jump;//up (gravity affected)
        bool can_fall;//down (gravity)
        float node_dist;
    };

    path_options default_opt();

    typedef std::function<bool(glm::vec3)> allowed_at_pos_func;
    typedef std::function<glm::vec3(allowed_at_pos_func,glm::vec3,glm::vec3,path_options)> pathfinder_func;//allowed_at,start,end,opt

    glm::vec3 pathfind_astar(allowed_at_pos_func allowed_at,glm::vec3 start,glm::vec3 end,const path_options& opt);

}

#endif //GLMINECRAFT3_PATHFINDING_H
