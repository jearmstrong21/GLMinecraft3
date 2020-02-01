//
// Created by Jack Armstrong on 1/31/20.
//

#ifndef GLMINECRAFT3_ENTITIES_H
#define GLMINECRAFT3_ENTITIES_H

#include "entity.h"
#include "entity_player.h"
#include "entity_zombie.h"

namespace entity {

    entity_ptr load(const nbt::nbt_compound_ptr&tag);

}

#endif //GLMINECRAFT3_ENTITIES_H
