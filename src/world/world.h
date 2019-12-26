//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_WORLD_H
#define GLMINECRAFT3_WORLD_H

#include "block/blocks.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "utils/noise/noise.h"
#include "utils/utils.h"

#define WORLD_SIZE 2

namespace block {
    struct world {

        std::shared_ptr<chunk> map[WORLD_SIZE][WORLD_SIZE];

        world();

        void generate_world();

        [[nodiscard]] bool in_bounds(int x, int y, int z)const;

        [[nodiscard]] bool in_bounds(glm::ivec3 v)const;

        [[nodiscard]] block_state get(int x, int y, int z)const;

        [[nodiscard]] block_state get(glm::ivec3 v)const;

        void set(int x, int y, int z, block_state b);

        void set(glm::ivec3 v, block_state b);

        [[nodiscard]] block_context get_block_context(glm::ivec3 p)const;

    };

}

#endif //GLMINECRAFT3_CLIENT_WORLD_H
