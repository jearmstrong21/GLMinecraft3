//
// Created by Jack Armstrong on 1/31/20.
//

#include "entities.h"

namespace entity {

    entity_ptr load(const nbt::nbt_compound_ptr &tag) {
        int id = tag->value["entity_type_id"]->as_int();
        if (id == ENTITY_ID_ZOMBIE) {
            entity_ptr ent(new entity_zombie());
            ent->load(tag);
            return ent;
        }
        if (id == ENTITY_ID_PLAYER) {
            entity_ptr ent(new entity_player());
            ent->load(tag);
            return ent;
        }
        if (id == ENTITY_ID_PIG) {
            entity_ptr ent(new entity_pig());
            ent->load(tag);
            return ent;
        }
        return nullptr;
    }

}