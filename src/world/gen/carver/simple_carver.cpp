//
// Created by Jack Armstrong on 11/19/19.
//

#include "simple_carver.h"

namespace world::gen {

    simple_carver::simple_carver(uint32_t seed):base_carver(seed) {

    }

    void simple_carver::carve(block::world* world, void (block::world::*set)(int x, int y, int z, block::block_state bs)) {

    }

}