//
// Created by Jack Armstrong on 1/31/20.
//

#ifndef GLMINECRAFT3_REGISTRY_H
#define GLMINECRAFT3_REGISTRY_H

#include "item.h"
#include "item_sword.h"
#include "item_none.h"
#include "item_block.h"

namespace item {

    struct item_registry {

        static item *DIAMOND_SWORD;
        static item *GOLD_SWORD;
        static item *IRON_SWORD;
        static item *STONE_SWORD;
        static item *WOOD_SWORD;
        static item *NONE;

        static item *BLOCK_BRICKS;
        static item *BLOCK_DIRT;
        static item *BLOCK_GRASS;
        static item *BLOCK_STONE;

        static std::map<int, item *> map;

        static void initialize();

        static void free();

    };

}

#endif //GLMINECRAFT3_REGISTRY_H
