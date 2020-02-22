//
// Created by Jack Armstrong on 2/22/20.
//

#ifndef GLMINECRAFT3_ENTITY_AI_H
#define GLMINECRAFT3_ENTITY_AI_H

#include <glm/glm.hpp>
#include <utils/aabb.h>

namespace entity {

    struct entity;

}

namespace entity::ai {

    glm::vec3 get_motion(entity *agent, glm::vec3 goal);

}

#endif //GLMINECRAFT3_ENTITY_AI_H
