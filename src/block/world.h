//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_WORLD_H
#define GLMINECRAFT3_WORLD_H

#include "blocks.h"

#define WORLD_SIZE 2

namespace block {

    struct World {

        Chunk map[WORLD_SIZE][WORLD_SIZE];

        World();

        [[nodiscard]] bool in_bounds(int x, int y, int z)const;

        [[nodiscard]] bool in_bounds(glm::ivec3 v)const;

        [[nodiscard]] BlockState get(int x, int y, int z)const;

        [[nodiscard]] BlockState get(glm::ivec3 v)const;

        void set(int x, int y, int z, BlockState b);

        void set(glm::ivec3 v, BlockState b);

        [[nodiscard]] BlockContext getCTX(glm::ivec3 p)const;

    };

}

#endif //GLMINECRAFT3_WORLD_H
