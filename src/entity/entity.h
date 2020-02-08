//
// Created by Jack Armstrong on 1/16/20.
//

#ifndef GLMINECRAFT3_ENTITY_H
#define GLMINECRAFT3_ENTITY_H

#include <utility>

#include "utils/aabb.h"
#include "nbt/nbt.h"
#include "entity_id.h"
#include "utils/utils.h"
#include <memory>
#include <world/world.h>

namespace server { struct game_room; }

namespace entity {

    struct entity {

        DATA int type_id;
        DATA utils::aabb box{};
        DATA glm::vec3 velocity{};
        DATA glm::vec3 motion{};
        DATA std::string uuid;
        DATA glm::vec3 lookdir{};
        DATA glm::vec3 facedir{};

        TRANSIENT server::game_room *server;
        TRANSIENT std::chrono::nanoseconds last_time = std::chrono::nanoseconds::zero();
        TRANSIENT bool first_update = true;
        TRANSIENT bool grounded = false;

        explicit entity(int type_id);

        virtual void save_additional_information(const nbt::nbt_compound_ptr &tag) = 0;

        virtual void load_additional_information(const nbt::nbt_compound_ptr &tag) = 0;

        virtual bool can_go_through_block(block::block_state state);

        void save(const nbt::nbt_compound_ptr &tag);

        void load(const nbt::nbt_compound_ptr &tag);

        bool collides_with_block_at(glm::vec3 point);

        void jump();

        void apply_physics();

        virtual void handle_ai() = 0;

        void update();

    };

    typedef std::shared_ptr<entity> entity_ptr;

}

#endif //GLMINECRAFT3_ENTITY_H
