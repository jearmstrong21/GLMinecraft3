//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_WORLD_H
#define GLMINECRAFT3_WORLD_H

#include "blocks.h"

#define WORLD_SIZE 16

namespace block {

    struct World {

        Chunk map[WORLD_SIZE][WORLD_SIZE];

        World();

        bool in_bounds(int x, int y, int z);

        bool in_bounds(glm::ivec3 v);

        BlockState get(int x, int y, int z);

        BlockState get(glm::ivec3 v);

        void set(int x, int y, int z, BlockState b);

        void set(glm::ivec3 v, BlockState b);

    };

}

#endif //GLMINECRAFT3_WORLD_H
