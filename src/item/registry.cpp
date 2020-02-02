//
// Created by Jack Armstrong on 1/31/20.
//

#include "registry.h"

namespace item {

    std::shared_ptr<item> item_registry::NONE;
    std::shared_ptr<item> item_registry::DIAMOND_SWORD;
    std::shared_ptr<item> item_registry::GOLD_SWORD;
    std::shared_ptr<item> item_registry::IRON_SWORD;
    std::shared_ptr<item> item_registry::STONE_SWORD;
    std::shared_ptr<item> item_registry::WOOD_SWORD;

    std::map<int, std::shared_ptr<item>> item_registry::map;

    void item_registry::initialize() {
        auto put = [&](const std::shared_ptr<item> &i) {
            map[i->properties.item_type_id] = i;
        };

        int id = 0;
        auto i = [&]() {
            return id++;
        };

        put(NONE = std::shared_ptr<item>(new item_none({i(), "none"})));
        put(DIAMOND_SWORD = std::shared_ptr<item>(new item_sword({i(), "diamond_sword"}, enums::material::DIAMOND)));
        put(GOLD_SWORD = std::shared_ptr<item>(new item_sword({i(), "gold_sword"}, enums::material::GOLD)));
        put(IRON_SWORD = std::shared_ptr<item>(new item_sword({i(), "iron_sword"}, enums::material::IRON)));
        put(STONE_SWORD = std::shared_ptr<item>(new item_sword({i(), "stone_sword"}, enums::material::STONE)));
        put(WOOD_SWORD = std::shared_ptr<item>(new item_sword({i(), "wood_sword"}, enums::material::WOOD)));
    }
}