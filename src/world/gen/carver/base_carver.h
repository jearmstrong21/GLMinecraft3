//
// Created by paladin on 11/19/19.
//

#ifndef GLMINECRAFT3_BASE_CARVER_H
#define GLMINECRAFT3_BASE_CARVER_H


/**
 * CARVERS
 * Carvers have 3 main responsibilities in a Notchian server:
 * + Generate ravines
 * + Generate caves
 * + Generate lakes/lava lakes
 */
#include "world/world.h"

namespace world::gen {
    class base_carver {
    private:
        uint32_t seed;
    public:
        explicit base_carver(uint32_t seed);
        virtual void carve(block::world* world, void (block::world::*set)(int x, int y, int z, block::block_state bs))=0;
    };

}

#endif //GLMINECRAFT3_BASE_CARVER_H
