//
// Created by paladin on 11/19/19.
//

#include "base_carver.h"

world::gen::base_carver::base_carver(int seed) {
    this->seed = seed;
}

void world::gen::base_carver::carve(block::World* world, void (block::World::* set)(int, int, int, block::BlockState)) {

}
