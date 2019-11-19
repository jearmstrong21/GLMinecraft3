//
// Created by paladin on 11/19/19.
//

#include <block/types/block_grass.h>
#include <block/types/block_dirt.h>
#include <block/types/block_stone.h>
#include "base_surface.h"

world::gen::base_surface::base_surface(int seed) {

}

block::BlockState world::gen::base_surface::getForLocation(int x, int y, int z, int h) {
    if (y == h) {
        return block::GRASS.defaultState;
    } else if (y > h - 5) {
        return block::DIRT.defaultState;
    }

    return block::STONE.defaultState;
}
