//
// Created by Jack Armstrong on 1/31/20.
//

#include "registry.h"

namespace item {

    std::shared_ptr<item> item_registry::DIAMOND_SWORD;
    std::map<int, std::shared_ptr<item>> item_registry::map;

    void item_registry::initialize() {
        auto put = [&](const std::shared_ptr<item> &i) {
            map[i->properties.item_type_id] = i;
        };

        put(DIAMOND_SWORD = std::shared_ptr<item>(new item_sword({1, "diamond_sword"})));
    }
}