//
// Created by Jack Armstrong on 1/16/20.
//

#ifndef GLMINECRAFT3_ENTITY_H
#define GLMINECRAFT3_ENTITY_H

#include <utility>

#include "aabb.h"
#include "nbt/nbt.h"
#include "entity_id.h"
#include "utils/utils.h"
#include <memory>
#include <world/world.h>

namespace server { struct game_room; }

namespace entity {

    struct entity {

        int type_id;
        aabb box{};
        glm::vec3 velocity{};
        glm::vec3 motion{};
        std::string uuid;
        glm::vec3 lookdir{};

        TRANSIENT server::game_room *server;

        explicit entity(int type_id);

        virtual void save_additional_information(const nbt::nbt_compound_ptr& tag) = 0;

        virtual void load_additional_information(const nbt::nbt_compound_ptr& tag) = 0;

        virtual bool can_go_through_block(block::block_state state);

        void save(const nbt::nbt_compound_ptr& tag);

        void load(const nbt::nbt_compound_ptr& tag);

        bool collides_with_block_at(glm::vec3 point);

        TRANSIENT std::chrono::nanoseconds last_time = std::chrono::nanoseconds::zero();
        TRANSIENT bool first_update = true;
        TRANSIENT bool grounded = false;

        void apply_physics();

        virtual void handle_ai() = 0;

        void update();

    };

    typedef std::shared_ptr<entity> entity_ptr;

}

#endif //GLMINECRAFT3_ENTITY_H
