//
// Created by Jack Armstrong on 11/26/19.
//

#ifndef GLMINECRAFT3_ENTITY_MANAGER_H
#define GLMINECRAFT3_ENTITY_MANAGER_H

#include "entity.h"

namespace server {

    struct entity_manager {

        std::vector<std::shared_ptr<nbt::nbt>>entities;

    };

}

#endif //GLMINECRAFT3_ENTITY_MANAGER_H
