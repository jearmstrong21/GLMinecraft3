//
// Created by Jack Armstrong on 1/26/20.
//

#include "entity_zombie.h"

namespace entity {

    entity_zombie::entity_zombie(std::string id, server::game_room *s, glm::vec3 spawnpos) : entity(std::move(id), s) {
        box.pos = spawnpos;
        box.size = glm::vec3{0.6, 1.95, 0.6};
    }

    void entity_zombie::save_additional_information(const nbt::nbt_compound_ptr &tag) {
        tag->value["entity_type_id"] = nbt::nbt_int::make(ENTITY_ID_ZOMBIE);
    }

    void entity_zombie::load_additional_information(const nbt::nbt_compound_ptr &tag) {
        ASSERT_OR_EXIT(tag->value["entity_type_id"]->as_int() == ENTITY_ID_ZOMBIE,
                       "Entity type id as loaded by entity_zombie was incorrect.");
    }

    void entity_zombie::handle_ai() {
        entity *player = nullptr;
    }


}