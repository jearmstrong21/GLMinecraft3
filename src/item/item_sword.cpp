//
// Created by Jack Armstrong on 1/31/20.
//

#include "item_sword.h"

#include <utility>

namespace item {

    item_sword::item_sword(item_properties properties, enums::material material) : item(std::move(properties)),
                                                                                   material(std::move(material)) {

    }

    void item_sword::attack(const item_use_context &ctx) {

    }

    void item_sword::use(const item_use_context &ctx) {

    }

    client::item_texture_descr item_sword::render(item_stack stack) {
        return {{{material.sword_texture, {1, 1, 1}}}, stack.count};
    }

    void item_sword::save_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) {

    }

    void item_sword::load_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) {

    }

    item_stack item_sword::make() {
        return item_stack{properties.item_type_id, 1};
    }

}