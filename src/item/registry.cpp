//
// Created by Jack Armstrong on 1/31/20.
//

#include "registry.h"
#include "block/blocks.h"

namespace item {

    item *item_registry::NONE;
    item *item_registry::DIAMOND_SWORD;
    item *item_registry::GOLD_SWORD;
    item *item_registry::IRON_SWORD;
    item *item_registry::STONE_SWORD;
    item *item_registry::WOOD_SWORD;

    item *item_registry::BLOCK_BRICKS;
    item *item_registry::BLOCK_DIRT;
    item *item_registry::BLOCK_GRASS;
    item *item_registry::BLOCK_STONE;

    std::map<int, item *> item_registry::map;

    void item_registry::initialize() {
        auto put = [&](item *i) {
            map[i->properties.item_type_id] = i;
        };

        int id = 0;
        auto i = [&]() {
            return id++;
        };

        put(NONE = (new item_none({i(), "none"})));
        put(DIAMOND_SWORD = (new item_sword({i(), "diamond_sword"}, enums::material::DIAMOND)));
        put(GOLD_SWORD = (new item_sword({i(), "gold_sword"}, enums::material::GOLD)));
        put(IRON_SWORD = (new item_sword({i(), "iron_sword"}, enums::material::IRON)));
        put(STONE_SWORD = (new item_sword({i(), "stone_sword"}, enums::material::STONE)));
        put(WOOD_SWORD = (new item_sword({i(), "wood_sword"}, enums::material::WOOD)));

        put(BLOCK_BRICKS = (new item_block_default({i(), "block_bricks"}, block::BRICKS.defaultState,
                                                   {{{client::atlas_texture::BRICKS, glm::vec3{1,
                                                                                               1,
                                                                                               1}}}})));
        put(BLOCK_DIRT = (new item_block_default({i(), "block_dirt"}, block::DIRT.defaultState,
                                                 {{{client::atlas_texture::DIRT, glm::vec3{1, 1, 1}}}})));
        put(BLOCK_GRASS = (new item_block_default({i(), "block_grass"}, block::GRASS.defaultState,
                                                  {{{client::atlas_texture::GRASS_SIDE, glm::vec3(
                                                          1, 1,
                                                          1)}, {client::atlas_texture::GRASS_SIDE_OVERLAY, glm::vec3(
                                                          0.47, 0.82, 0.37)}}})));
        put(BLOCK_STONE = (new item_block_default({i(), "block_stone"}, block::STONE.defaultState,
                                                  {{{client::atlas_texture::STONE, glm::vec3{1, 1, 1}}}})));
    }

    void item_registry::free() {
        for (auto &p:map) {
            delete p.second;
        }
    }
}