//
// Created by Jack Armstrong on 2/1/20.
//

#include "item_none.h"

#include <utility>

namespace item {

    item_none::item_none(item_properties properties) : item(std::move(properties)) {

    }

    client::item_texture_descr item_none::render(item_stack stack) {
        return client::item_texture_descr{{{client::atlas_texture::UNKNOWN, glm::vec3{1}}}, -1};
    }

    item_stack item_none::make() {
        return item_stack();
    }

}