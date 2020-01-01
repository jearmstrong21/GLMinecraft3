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
    };

    struct entity_type_base : entity_type {
        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize() const override;

        void update(std::shared_ptr<nbt::nbt> data, game_room *room) const override;
    };

    struct entity_type_player : entity_type_base {
        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize() const override;

        void update(std::shared_ptr<nbt::nbt> data, game_room *room) const override;
    };

}

#endif //GLMINECRAFT3___ENTITY_H
