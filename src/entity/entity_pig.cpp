//
// Created by Jack Armstrong on 2/20/20.
//


#include <server/game_room.h>
#include "entity_pig.h"
#include <algorithm>

namespace entity {

    entity_pig::entity_pig() : entity(ENTITY_ID_PIG) {
        box.size = {0.9, 0.9, 0.9};
    }

    entity_ptr entity_pig::spawn(std::string id, glm::vec3 pos, server::game_room *server) {
        std::shared_ptr<entity_pig> p = std::make_shared<entity_pig>();
        p->uuid = std::move(id);
        p->box.pos = pos;
        p->server = server;
        return std::dynamic_pointer_cast<entity>(p);
    }

    void entity_pig::save_additional_information(const nbt::nbt_compound_ptr &tag) {

    }

    void entity_pig::load_additional_information(const nbt::nbt_compound_ptr &tag) {

    }

    void entity_pig::handle_ai() {
    }


}