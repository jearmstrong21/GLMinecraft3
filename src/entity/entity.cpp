//
// Created by Jack Armstrong on 1/26/20.
//

#include <glm/gtx/rotate_vector.hpp>
#include "entity.h"
#include "server/game_room.h"

namespace entity {

    entity::entity(int type_id) : type_id(type_id) {
        lookdir = {1, 0, 0};
        facedir = {1, 0, 0};
    }

    void entity::jump() {
        if (grounded) {
            grounded = false;
            velocity.y = 300;
        }
    }

    bool entity::can_go_through_block(block::block_state state) {
        return state == 0;
    }

    void entity::save(const nbt::nbt_compound_ptr &tag) {
        tag->value["entity_type_id"] = nbt::nbt_int::make(type_id);
        tag->value["uuid"] = nbt::nbt_string::make(uuid);
        tag->value["box"] = nbt::nbt_compound::make({});
        box.save(nbt::cast_compound(tag->value["box"]));
        tag->value["motion"] = utils::cast3(motion);
        tag->value["lookdir"] = utils::cast3(lookdir);
        tag->value["velocity"] = utils::cast3(velocity);
        tag->value["facedir"] = utils::cast3(facedir);
        save_additional_information(tag);
    }

    void entity::load(const nbt::nbt_compound_ptr &tag) {
        ASSERT_OR_EXIT(tag->value["entity_type_id"]->as_int() == type_id, "Wrong entity_type_id");
        uuid = tag->value["uuid"]->as_string();
        box.load(nbt::cast_compound(tag->value["box"]));
        motion = utils::cast3(tag->value["motion"]);
        lookdir = utils::cast3(tag->value["lookdir"]);
        velocity = utils::cast3(tag->value["velocity"]);
        facedir = utils::cast3(tag->value["facedir"]);
        load_additional_information(tag);
    }

    void entity::apply_physics() {
        float dt = 0;
        std::chrono::nanoseconds cur_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch());
        if (first_update) {
            first_update = false;
        } else {
            dt = (float) (cur_time - last_time).count() / 1000000000.0f;
        }
        last_time = cur_time;
        velocity.y -= 1000 * dt;
        motion += velocity * dt;
        auto trace_pos = [&](glm::vec3 start, glm::vec3 dir, float cur_dt, int iters) -> glm::vec3 {
            for (int _ = 0; _ < iters; _++) {
                cur_dt *= 0.5;
                if (!collides_with_block_at(start + dir * cur_dt))start += dir * cur_dt;
            }
            return start;
        };

        int iters = 10;
        if (motion.y != 0) {
            glm::vec3 orig = box.pos;
            box.pos = trace_pos(box.pos, {0, motion.y, 0}, dt, iters);
            if (orig == box.pos)velocity.y = 0;
            grounded = orig == box.pos;
        }
        if (motion.x != 0) {
            box.pos = trace_pos(box.pos, {motion.x, 0, 0}, dt, iters);
        }
        if (motion.z != 0) {
            box.pos = trace_pos(box.pos, {0, 0, motion.z}, dt, iters);
        }
        if (glm::length(glm::vec3{motion.x, 0, motion.z}) > 0) {
            facedir = glm::normalize(glm::vec3{motion.x, 0, motion.z});
        }
    }

    void entity::update() {
        apply_physics();
        handle_ai();
    }

    bool entity::collides_with_block_at(glm::vec3 point) {
        bool collide = false;
        utils::aabb{point, box.size}.foreach([&](glm::ivec3 i) {
            if (!can_go_through_block(server->world.get(i)))collide = true;
        });
        return collide;
    }

}
