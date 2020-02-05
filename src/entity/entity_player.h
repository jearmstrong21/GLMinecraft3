//
// Created by Jack Armstrong on 1/26/20.
//

#ifndef GLMINECRAFT3_ENTITY_PLAYER_H
#define GLMINECRAFT3_ENTITY_PLAYER_H

#include "entity.h"
#include "item/item.h"

namespace entity {

    struct entity_player : entity {

    private:
        static const int inventory_size = 36;

    public:

        item::item_stack inventory[inventory_size]{};
        int selected_item;

        entity_player();

        void save_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void load_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void handle_ai() override;

        static entity_ptr spawn(std::string uuid, glm::vec3 pos, server::game_room *server);

    };

}

#endif //GLMINECRAFT3_ENTITY_PLAYER_H
