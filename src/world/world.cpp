//
// Created by Jack Armstrong on 11/16/19.
//

#include <memory>
#include "world/gen/surface/simple_grass_surface.h"
#include "world/gen/carver/simple_carver.h"
#include "world.h"

namespace block {
    world::world() {
        int seed = 5;
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                map[x][z] = std::make_shared<chunk>();
            }
        }

        std::shared_ptr<utils::noise::perlin> perlin = std::make_shared<utils::noise::perlin>(seed);
        double zoom = 0.025;

//        std::shared_ptr<::world::gen::base_surface>surface=std::shared_ptr<::world::gen::base_surface>((::world::gen::base_surface*)new ::world::gen::simple_grass_surface(seed));
//        std::shared_ptr<::world::gen::base_surface>surface=std::make_shared<::world::gen::simple_grass_surface>(seed);
        ::world::gen::simple_grass_surface surface(seed);
        ::world::gen::simple_carver carver(seed);
//        ::world::gen::base_surface surface(seed);
//        ::world::gen::base_carver carver(seed);

        for (int x = 0; x < WORLD_SIZE * 16; x++) {
            for (int z = 0; z < WORLD_SIZE * 16; z++) {

                int h = (int) (perlin->get(x * zoom, z * zoom, 0) * 100);
                // TODO: better height calc
                if (h < 10)h = 10;
                if (h > 100)h = 100;

                for (int y = 0; y <= h; y++) {
                    block_state bs = surface.get_for_location(x, y, z, h);
                    if (bs == 0)continue;
                    set(x, y, z, bs);
                }
            }
        }

        carver.carve(this, &world::set);
    }

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

}