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

        World() {
            for (int x = 0; x < WORLD_SIZE; x++) {
                for (int z = 0; z < WORLD_SIZE; z++) {
                    for (int a = 0; a < 16; a++) {
                        int h = x + z;
                        for (int b = 0; b < 16; b++) {
                            for (int y = 0; y <= h; y++) {
                                BlockState b;
                                if (y == h)b = GRASS.getDefaultState();
                            }
                        }
                    }
                }
            }
        }

        bool in_bounds(int x, int y, int z) {
            return x >= 0 && y >= 0 && z >= 0 && x < WORLD_SIZE * 16 && y < 256 && z < WORLD_SIZE * 16;
        }

        bool in_bounds(glm::ivec3 v) { return in_bounds(v.x, v.y, v.z); };

        BlockState get(int x, int y, int z) {
            if (in_bounds(x, y, z))return map[x / 16][z / 16].get(x % 16, y, z % 16);
            return 0;
        }

        BlockState get(glm::ivec3 v) { return get(v.x, v.y, v.z); }

        void set(int x, int y, int z, BlockState b) {
            if (in_bounds(x, y, z))map[x / 16][z / 16].set(x % 16, y, z % 16, b);
        }

        void set(glm::ivec3 v, BlockState b) { set(v.x, v.y, v.z, b); }

    };

}

#endif //GLMINECRAFT3_WORLD_H
