//
// Created by Jack Armstrong on 1/31/20.
//

#include "item_sword.h"

#include <utility>

namespace item {

    item_sword::item_sword(item_properties properties, enums::material material) : item(std::move(properties)),
                                                                                   material(std::move(material)) {

    }

    client::item_texture_descr item_sword::render(item_stack stack) {
        return {{{material.sword_texture, {1, 1, 1}}}, stack.count};
    }

    item_stack item_sword::make() {
        item_stack s;
        s.item_type_id = properties.item_type_id;
        s.count = 1;
        return s;
    }

}