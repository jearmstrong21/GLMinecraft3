//
// Created by Jack Armstrong on 1/17/20.
//

#ifndef GLMINECRAFT3_ENTITY_PHYSICS_H
#define GLMINECRAFT3_ENTITY_PHYSICS_H

#include "entity.h"

namespace entity {

    struct game_room;

    void handle_physics(const entity_ptr &entity, game_room *room);

}

#endif //GLMINECRAFT3_ENTITY_PHYSICS_H
