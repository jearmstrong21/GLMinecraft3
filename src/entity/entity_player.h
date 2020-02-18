//
// Created by Jack Armstrong on 1/26/20.
//

#ifndef GLMINECRAFT3_ENTITY_PLAYER_H
#define GLMINECRAFT3_ENTITY_PLAYER_H

#include "server/delayed_task.h"
#include "entity.h"
#include "item/item.h"

namespace entity {

    struct entity_player : entity {

    private:
        TRANSIENT static const int inventory_size = 36;

    public:

        DATA item::item_stack inventory[inventory_size]{};
        DATA int selected_item;

        DATA bool leftclick;
        DATA bool rightclick;
        DATA bool firstperson;

        DATA int last_use_tick;
        DATA int last_attack_tick;

        TRANSIENT block::intersection intersection;

        static constexpr const float eye_height = 1.75;
        static constexpr const int USE_DELAY = 10 TICKS;
        static constexpr const int ATTACK_DELAY = 10 TICKS;

        item::item_stack *get_held_item();

        item::item_use_context get_use_context();

        entity_player();

        void attempt_use();

        void attempt_attack();

        void leftclick_start();

        void leftclick_continue();

        void leftclick_end();

        void rightclick_start();

        void rightclick_continue();

        void rightclick_end();

        void save_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void load_additional_information(const nbt::nbt_compound_ptr &tag) override;

        void handle_ai() override;

        static entity_ptr spawn(std::string uuid, glm::vec3 pos, server::game_room *server);

    };

    enum click_type {
        LEFT_START,
        LEFT_CONTINUE,
        LEFT_END,
        RIGHT_START,
        RIGHT_CONTINUE,
        RIGHT_END
    };

    struct task_player_click_interact : server::delayed_task {
        entity_player *player;
        click_type type;

        task_player_click_interact(entity_player *player, click_type type) : player(player), type(type) { }

        ~task_player_click_interact() override = default;

        void execute() override {
            switch (type) {
                case LEFT_START:
                    player->leftclick_start();
                    break;
                case LEFT_CONTINUE:
                    player->leftclick_continue();
                    break;
                case LEFT_END:
                    player->leftclick_end();
                    break;
                case RIGHT_START:
                    player->rightclick_start();
                    break;
                case RIGHT_CONTINUE:
                    player->rightclick_continue();
                    break;
                case RIGHT_END:
                    player->rightclick_end();
                    break;
            }
        }
    };

}

#endif //GLMINECRAFT3_ENTITY_PLAYER_H
