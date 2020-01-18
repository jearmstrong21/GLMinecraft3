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

namespace entity {

    struct entity {

        aabb box;
        glm::vec3 velocity;
        glm::vec3 motion;
        std::string id;
        glm::vec3 lookdir;

        entity(std::string id) : id(std::move(id)) {

        }

        virtual void save_additional_information(const nbt::nbt_compound_ptr &tag) = 0;

        virtual void load_additional_information(const nbt::nbt_compound_ptr &tag) = 0;

        void save(const nbt::nbt_compound_ptr &tag) {
            tag->value["id"] = nbt::nbt_string::make(id);
            tag->value["position"] = utils::cast3(box.pos);
            tag->value["bbsize"] = utils::cast3(box.size);
            tag->value["motion"] = utils::cast3(motion);
            tag->value["lookdir"] = utils::cast3(lookdir);
            tag->value["velocity"] = utils::cast3(velocity);
            save_additional_information(tag);
        }

        void load(const nbt::nbt_compound_ptr &tag) {
            id = tag->value["id"]->as_string();
            box.pos = utils::cast3(tag->value["position"]);
            box.size = utils::cast3(tag->value["bbsize"]);
            motion = utils::cast3(tag->value["motion"]);
            lookdir = utils::cast3(tag->value["lookdir"]);
            velocity = utils::cast3(tag->value["velocity"]);
            load_additional_information(tag);
        }

    };

    struct entity_player : entity {

        entity_player(std::string id, glm::vec3 spawnpos) : entity(std::move(id)) {
            box.pos = spawnpos;
            box.size = glm::vec3{0.6, 1.5, 0.6};
        }

        void save_additional_information(const nbt::nbt_compound_ptr &tag) override {
            tag->value["entity_type_id"] = nbt::nbt_int::make(ENTITY_ID_PLAYER);
        }

        void load_additional_information(const nbt::nbt_compound_ptr &tag) override {
            ASSERT_OR_EXIT(tag->value["entity_type_id"]->as_int() == ENTITY_ID_PLAYER,
                           "Entity type id as loaded by entity_player was incorrect.");
        }

    };

    typedef std::shared_ptr<entity> entity_ptr;

}

#endif //GLMINECRAFT3_ENTITY_H
