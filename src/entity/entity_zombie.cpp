//
// Created by Jack Armstrong on 1/26/20.
//

#include <server/game_room.h>
#include "entity_zombie.h"
#include "entity_ai.h"
#include <algorithm>

namespace entity {

    entity_zombie::entity_zombie() : entity(ENTITY_ID_ZOMBIE) {
        box.size = {0.6, 1.95, 0.6};
    }

    entity_ptr entity_zombie::spawn(std::string id, glm::vec3 pos, server::game_room *server) {
        std::shared_ptr<entity_zombie> p = std::make_shared<entity_zombie>();
        p->uuid = std::move(id);
        p->box.pos = pos;
        p->server = server;
        return std::dynamic_pointer_cast<entity>(p);
    }

    void entity_zombie::save_additional_information(const nbt::nbt_compound_ptr &tag) {

    }

    void entity_zombie::load_additional_information(const nbt::nbt_compound_ptr &tag) {

    }

    void entity_zombie::handle_ai() {
        entity_ptr player = nullptr;
        for (const auto &p:server->entities) {
            if (p.second->type_id == ENTITY_ID_PLAYER) {
                if (player == nullptr)player = p.second;
                if (glm::distance(box.pos, player->box.pos) < glm::distance(box.pos, p.second->box.pos)) {
                    player = p.second;
                }
            }
        }
        if (player == nullptr) {
            return;
        }

        motion = glm::vec3{0};
        motion = ai::get_motion(this, player->box.pos);

        if (motion.y > 0 && grounded) {
            jump();
            grounded = false;
        }
        motion.y = 0;

    }


}