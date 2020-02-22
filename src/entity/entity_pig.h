//
// Created by Jack Armstrong on 2/20/20.
//

#ifndef GLMINECRAFT3_ENTITY_PIG_H
#define GLMINECRAFT3_ENTITY_PIG_H

#include "entity.h"

namespace entity {

    struct entity_pig : entity {

        entity_pig();

        void save_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void load_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void handle_ai() override;

        static entity_ptr spawn(std::string id, glm::vec3 pos, server::game_room *server);

    };

}

#endif //GLMINECRAFT3_ENTITY_PIG_H
