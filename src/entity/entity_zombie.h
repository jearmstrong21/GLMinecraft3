//
// Created by Jack Armstrong on 1/26/20.
//

#ifndef GLMINECRAFT3_ENTITY_ZOMBIE_H
#define GLMINECRAFT3_ENTITY_ZOMBIE_H

#include "entity.h"

namespace entity {

    struct entity_zombie : entity {

        entity_zombie(std::string id, server::game_room *s, glm::vec3 spawnpos);

        void save_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void load_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void handle_ai() override;

    };

}

#endif //GLMINECRAFT3_ENTITY_ZOMBIE_H
