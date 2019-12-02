//
// Created by Jack Armstrong on 11/24/19.
//

#ifndef GLMINECRAFT3_SERVER_H
#define GLMINECRAFT3_SERVER_H

#include "world/world.h"
#include "entity_manager.h"
#include "entity.h"

namespace server {

    struct server {

        std::shared_ptr<block::world>world;
        std::shared_ptr<entity_manager>entities;

    };

}

#endif //GLMINECRAFT3_SERVER_H
