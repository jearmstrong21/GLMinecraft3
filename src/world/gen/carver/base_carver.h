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
#include <world/world.h>

namespace world::gen {
    class base_carver {
    private:
        int seed;
    public:
        explicit base_carver(int seed);
        void carve(block::World* world, void (block::World::*set)(int x, int y, int z, block::BlockState bs));
        // TODO: add set() function to signature so carver can set() blocks to air
        void carve(block::World* world, void (* set)(int, int, int, block::BlockState));
    };

}

#endif //GLMINECRAFT3_BASE_CARVER_H
