//
// Created by Jack Armstrong on 11/24/19.
//

#ifndef GLMINECRAFT3___ENTITY_H
#define GLMINECRAFT3___ENTITY_H

#include <glm/glm.hpp>
#include <nbt/nbt.h>

namespace server {

    struct game_room;

    struct entity_type {
        [[nodiscard]] virtual std::shared_ptr<nbt::nbt> initialize() const = 0;

        virtual void update(std::shared_ptr<nbt::nbt> data, game_room *room) const = 0;

        virtual ~entity_type() = default;
    };

    struct entity_type_base : entity_type {
        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize() const override;

        void update(std::shared_ptr<nbt::nbt> data, game_room *room) const override;

        ~entity_type_base() override = default;

        bool is_allowed_at_position(std::shared_ptr<nbt::nbt> data, glm::vec3 epos,server::game_room *room)const;
    };

    struct entity_type_player : entity_type_base {
        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize() const override;

        void update(std::shared_ptr<nbt::nbt> data, game_room *room) const override;

        ~entity_type_player() override = default;
    };

    struct entity_type_mob : entity_type_base {

    };

    struct entity_type_zombie : entity_type_mob {

        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize() const override;

        void update(std::shared_ptr<nbt::nbt> data, game_room *room) const override;

        ~entity_type_zombie() override = default;

    };

}

#endif //GLMINECRAFT3___ENTITY_H
