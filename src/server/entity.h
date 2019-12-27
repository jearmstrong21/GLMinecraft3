//
// Created by Jack Armstrong on 11/24/19.
//

#ifndef GLMINECRAFT3___ENTITY_H
#define GLMINECRAFT3___ENTITY_H

#include <glm/glm.hpp>
#include <nbt/nbt.h>
//#include "entity_ids.h"
namespace server {

    struct server;

    struct entity_type {

        int id;

        [[nodiscard]] virtual std::shared_ptr<nbt::nbt> initialize()const =0;
        virtual void update(std::shared_ptr<nbt::nbt>data,server*server)const =0;

        virtual void initialize_render()const =0;
        virtual void render(std::shared_ptr<nbt::nbt>entity)const =0;

    };

    struct entity_type_base: entity_type {
        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize() const override;
    };

    struct entity_type_player: entity_type_base {
        [[nodiscard]] std::shared_ptr<nbt::nbt> initialize()const override;
        void update(std::shared_ptr<nbt::nbt>data,server*server)const override;

        void initialize_render()const override;
        void render(std::shared_ptr<nbt::nbt>entity)const override;
    };

    std::shared_ptr<entity_type> get_entity_type(const std::shared_ptr<nbt::nbt>&data);

}

#endif //GLMINECRAFT3___ENTITY_H
