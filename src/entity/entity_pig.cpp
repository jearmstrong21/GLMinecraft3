//
// Created by Jack Armstrong on 2/20/20.
//


#include <server/game_room.h>
#include "entity_pig.h"
#include "entity_ai.h"
#include <algorithm>

namespace entity {

    entity_pig::entity_pig() : entity(ENTITY_ID_PIG) {
        box.size = {0.9, 0.9, 0.9};
        has_target = false;
        wander_target = {};
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
        if (rand() % 1000 <= 5) {
            has_target = true;
            int r = 20;
            int ry = 2;
            wander_target = box.pos + glm::vec3{rand() % r - r / 2.0f, rand() % ry - ry / 2.0f, rand() % r - r / 2.0f};
        }
        if (has_target) {
            motion = glm::vec3{0};
            motion = ai::get_motion(this, wander_target);

            if (motion.y > 0 && grounded) {
                jump();
                grounded = false;
            }
            motion.y = 0;
        }
    }


}