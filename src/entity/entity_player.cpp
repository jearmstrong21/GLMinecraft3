//
// Created by Jack Armstrong on 1/26/20.
//

#include "entity_player.h"

namespace entity {

    entity_player::entity_player(std::string id, server::game_room *s, glm::vec3 spawnpos) : entity(std::move(id),
                                                                                                    s) {
        box.pos = spawnpos;
        box.size = glm::vec3{0.6, 1.8, 0.6};
    }

    void entity_player::save_additional_information(const nbt::nbt_compound_ptr &tag) {
        tag->value["entity_type_id"] = nbt::nbt_int::make(ENTITY_ID_PLAYER);
    }

    void entity_player::load_additional_information(const nbt::nbt_compound_ptr &tag) {
        ASSERT_OR_EXIT(tag->value["entity_type_id"]->as_int() == ENTITY_ID_PLAYER,
                       "Entity type id as loaded by entity_player was incorrect.");
    }

    void entity_player::handle_ai() {

    }

}