//
// Created by Jack Armstrong on 11/19/19.
//

#ifndef GLMINECRAFT3_SIMPLE_CARVER_H
#define GLMINECRAFT3_SIMPLE_CARVER_H

#include "base_carver.h"

namespace world::gen {

    class simple_carver : base_carver {

    public:

        explicit simple_carver(uint32_t seed);

        void carve(block::world *world, void (block::world::*set)(int x, int y, int z, block::block_state bs)) override;

    };

}

#endif //GLMINECRAFT3_SIMPLE_CARVER_H
