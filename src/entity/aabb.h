//
// Created by Jack Armstrong on 1/16/20.
//

#ifndef GLMINECRAFT3_AABB_H
#define GLMINECRAFT3_AABB_H

#include <glm/glm.hpp>

namespace entity {

    struct aabb {

        glm::vec3 pos, size;

        glm::vec3 min(){
            return pos-size*glm::vec3{0.5,0,0.5};
        }

        glm::vec3 max(){
            return pos+size*glm::vec3{0.5,1,0.5};
        }

    };

}

#endif //GLMINECRAFT3_AABB_H
