//
// Created by Jack Armstrong on 2/1/20.
//

#include "item_none.h"

#include <utility>

namespace item {

    item_none::item_none(item_properties properties) : item(std::move(properties)) {

    }

    void item_none::attack(const item_use_context &ctx) {

    }

    void item_none::use(const item_use_context &ctx) {

    }

    client::item_texture_descr item_none::render(item_stack stack) {
        return client::item_texture_descr{{{client::atlas_texture::UNKNOWN, glm::vec3{1}}}, -1};
    }

    void item_none::save_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) {

    }

    void item_none::load_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) {

    }

    item_stack item_none::make() {
        return item_stack{properties.item_type_id, 0};
    }

}