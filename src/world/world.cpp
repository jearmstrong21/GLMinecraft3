//
// Created by Jack Armstrong on 11/16/19.
//

#include <memory>
#include <world/gen/surface/base_surface.h>
#include <world/gen/carver/base_carver.h>
#include "world.h"

namespace block {
    World::World() {
        int seed = 5;
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                map[x][z] = std::make_shared<Chunk>();
            }
        }

        std::shared_ptr<utils::noise::Perlin> perlin = std::make_shared<utils::noise::Perlin>(seed);
        double zoom = 0.025;

        world::gen::base_surface surface(seed);
        world::gen::base_carver carver(seed);

        //Too much perlin noise segfaults?
        for (int x = 0; x < WORLD_SIZE * 16; x++) {
            for (int z = 0; z < WORLD_SIZE * 16; z++) {

                int h = (int) (perlin->perlin(x * zoom, z * zoom, 0) * 100);
                // TODO: better height calc
                if (h < 10)h = 10;
                if (h > 100)h = 100;

                for (int y = 0; y <= h; y++) {
                    BlockState bs = surface.getForLocation(x, y, z, h);
                    if (bs == 0)continue;
                    set(x, y, z, bs);
                }
            }
        }

        carver.carve(this, &World::set);
    }

    bool World::in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < WORLD_SIZE * 16 && y < 256 && z < WORLD_SIZE * 16;
    }

    bool World::in_bounds(glm::ivec3 v) const { return in_bounds(v.x, v.y, v.z); };

    BlockState World::get(int x, int y, int z) const {
        if (in_bounds(x, y, z))return map[x / 16][z / 16]->get(x % 16, y, z % 16);
        return 0;
    }

    BlockState World::get(glm::ivec3 v) const { return get(v.x, v.y, v.z); }

    void World::set(int x, int y, int z, BlockState b) {
        if (in_bounds(x, y, z))map[x / 16][z / 16]->set(x % 16, y, z % 16, b);
    }

    void World::set(glm::ivec3 v, BlockState b) { set(v.x, v.y, v.z, b); }

    BlockContext World::getCTX(glm::ivec3 p) const {
        return BlockContext{get(p.x - 1, p.y, p.z), get(p.x + 1, p.y, p.z), get(p.x, p.y - 1, p.z),
                            get(p.x, p.y + 1, p.z), get(p.x, p.y, p.z - 1), get(p.x, p.y, p.z + 1), get(p), p};
    }

}