//
// Created by paladin on 12/29/19.
//

#include <memory>
#include "block/blocks.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "utils/noise/noise.h"
#include "utils/utils.h"
#include "world_generator.h"
#include "../world.h"
#include "world/gen/surface/simple_grass_surface.h"
#include "world/gen/carver/simple_carver.h"

namespace block {
    void world_generator::generate_world(world *world) {
        printf("GENERATE WORLD THIS SHOULD ONLY PRINT ON SERVER\n");
        int seed = time(nullptr);

        std::shared_ptr<utils::noise::perlin> elevNoise = std::make_shared<utils::noise::perlin>(seed + 0);
        std::shared_ptr<utils::noise::perlin> roughNoise = std::make_shared<utils::noise::perlin>(seed + 1);
        std::shared_ptr<utils::noise::perlin> detailNoise = std::make_shared<utils::noise::perlin>(seed + 2);
        double elevZoom = 0.0125;
        double roughZoom = 0.0025;
        double detailZoom = 0.1;

        int SCALE_FACTOR = 16;

        const int SIZE = WORLD_SIZE * 16 / SCALE_FACTOR + 1;

        double elevPoints[SIZE][SIZE];
        double roughPoints[SIZE][SIZE];
        double detailPoints[SIZE][SIZE];

        for (int x = 0; x < SIZE; x++) {
            for (int z = 0; z < SIZE; z++) {
                int rx = x * SCALE_FACTOR;
                int rz = z * SCALE_FACTOR;
                elevPoints[x][z] = elevNoise->get(rx * elevZoom, 0, rz * elevZoom);
                roughPoints[x][z] = roughNoise->get(rx * roughZoom, 0, rz * roughZoom);
                detailPoints[x][z] = detailNoise->get(rx * detailZoom, 0, rz * detailZoom);
            }
        }

        auto interpolate = [&](int x, int z, int type) {
            int x0 = x / SCALE_FACTOR;
            int z0 = z / SCALE_FACTOR;
            double h00 = -1, h01 = -1, h10 = -1, h11 = -1;
            if (type == 0) {
                h00 = elevPoints[x0][z0];
                h01 = elevPoints[x0][z0 + 1];
                h10 = elevPoints[x0 + 1][z0];
                h11 = elevPoints[x0 + 1][z0 + 1];
            } else if (type == 1) {
                h00 = roughPoints[x0][z0];
                h01 = roughPoints[x0][z0 + 1];
                h10 = roughPoints[x0 + 1][z0];
                h11 = roughPoints[x0 + 1][z0 + 1];
            } else if (type == 2) {
                h00 = detailPoints[x0][z0];
                h01 = detailPoints[x0][z0 + 1];
                h10 = detailPoints[x0 + 1][z0];
                h11 = detailPoints[x0 + 1][z0 + 1];
            }

            double dx = (x % SCALE_FACTOR) / (double) SCALE_FACTOR;
            double dz = (z % SCALE_FACTOR) / (double) SCALE_FACTOR;

            double hx0 = utils::noise::lerp(h00, h10, dx);
            double hx1 = utils::noise::lerp(h01, h11, dx);

            double hf = utils::noise::lerp(hx0, hx1, dz);
            return hf;
        };

        ::world::gen::simple_grass_surface surface(seed);
        ::world::gen::simple_carver carver(seed);

        for (int x = 0; x < WORLD_SIZE * 16; x++) {
            for (int z = 0; z < WORLD_SIZE * 16; z++) {
                double elev = interpolate(x, z, 0);
                double rough = interpolate(x, z, 1);
                double detail = interpolate(x, z, 2);

                elev += rough * detail;

                int h = (int) (elev * 64 + 64);
                if (h < 0)h = 0;
                if (h >= 256)h = 256;

//                h=50;//TODO🎵What🎵can🎵I🎵say🎵except🎵delete🎵this🎵

                for (int y = 0; y <= h; y++) {
                    block_state bs = surface.get_for_location(x, y, z, h);
                    if (bs == 0)continue;
                    world->set(x, y, z, bs);
                }
            }
        }

        carver.carve(world, &world::set);
    }
}