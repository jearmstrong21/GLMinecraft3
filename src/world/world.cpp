//
// Created by Jack Armstrong on 11/16/19.
//

#include <memory>
#include <server/game_room.h>
#include "world.h"

namespace block {
    bool world::in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < WORLD_SIZE * 16 && y < 256 && z < WORLD_SIZE * 16;
    }

    bool world::in_bounds(glm::ivec3 v) const { return in_bounds(v.x, v.y, v.z); };

    block_state world::get(int x, int y, int z) const {
        if (in_bounds(x, y, z))return map[x / 16][z / 16]->get(x % 16, y, z % 16);
        return 0;
    }

    block_state world::get(glm::ivec3 v) const { return get(v.x, v.y, v.z); }

    void world::set(int x, int y, int z, block_state b) {
        if (in_bounds(x, y, z))map[x / 16][z / 16]->set(x % 16, y, z % 16, b);
    }

    void world::set(glm::ivec3 v, block_state b) { set(v.x, v.y, v.z, b); }

    block_context world::get_block_context(glm::ivec3 p) const {
        return block_context{get(p.x - 1, p.y, p.z), get(p.x + 1, p.y, p.z), get(p.x, p.y - 1, p.z),
                             get(p.x, p.y + 1, p.z), get(p.x, p.y, p.z - 1), get(p.x, p.y, p.z + 1), get(p), p};
    }

    world::world() {
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                map[x][z] = std::make_shared<chunk>();
            }
        }
    }

    void world::generate_world() {
        server::profiler()->push("world generate overworld");
        this->generator.generate_world(this);
        server::profiler()->pop();
    }
}