//
// Created by Jack Armstrong on 11/19/19.
//

#include "simple_grass_surface.h"

block::block_state world::gen::simple_grass_surface::get_for_location(int x, int y, int z, int h) {
    if (y == h) {
        return block::GRASS.defaultState;
    } else if (y > h - 5) {
        return block::DIRT.defaultState;
    }

    return block::STONE.defaultState;
}

world::gen::simple_grass_surface::simple_grass_surface(uint32_t seed) : base_surface(seed) {

}
