//
// Created by Jack Armstrong on 11/16/19.
//

#include <memory>
#include "world/gen/surface/simple_grass_surface.h"
#include "world/gen/carver/simple_carver.h"
#include "world.h"

namespace block {

    world::world() {
        int seed = time(0);
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                map[x][z] = std::make_shared<chunk>();
            }
        }

        std::shared_ptr<utils::noise::perlin> elevNoise = std::make_shared<utils::noise::perlin>(seed + 0);
        std::shared_ptr<utils::noise::perlin> roughNoise = std::make_shared<utils::noise::perlin>(seed + 1);
        std::shared_ptr<utils::noise::perlin> detailNoise = std::make_shared<utils::noise::perlin>(seed + 2);
        double elevZoom = 0.0125;
        double roughZoom = 0.0025;
        double detailZoom = 0.1;

        //(elevation + (roughness*detail))*64+64

        int SCALE_FACTOR = 8;

        const int SIZE = WORLD_SIZE * 16 / SCALE_FACTOR + 1;

        double elevPoints[SIZE][SIZE];
        double roughPoints[SIZE][SIZE];
        double detailPoints[SIZE][SIZE];

        //1T 0.1
        //2T 0.12
        //4T 0.12


        int numThreadSize=1;
        int threadSpan=SIZE/numThreadSize+1;
        std::vector<std::thread>threads;
        for(int __tx=0;__tx<numThreadSize;__tx++){
            for(int __tz=0;__tz<numThreadSize;__tz++){
                threads.emplace_back([&](int tx,int tz){
                    printf("S%i,%i\n",tx,tz);
                    int x0=threadSpan*tx;
                    int z0=threadSpan*tz;
                    int x1=threadSpan*tx+threadSpan;
                    int z1=threadSpan*tz+threadSpan;
                    for(int x=x0;x<x1;x++){
                        for(int z=z0;z<z1;z++){
                            if(x<0||z<0||x>=SIZE||z>=SIZE)continue;
                            int rx=x*SCALE_FACTOR;
                            int rz=z*SCALE_FACTOR;
                            elevPoints[x][z] = elevNoise->get(rx * elevZoom, 0, rz * elevZoom);
                            roughPoints[x][z] = roughNoise->get(rx * roughZoom, 0, rz * roughZoom);
                            detailPoints[x][z] = detailNoise->get(rx * detailZoom, 0, rz * detailZoom);
                        }
                    }
                    printf("E%i,%i\n",tx,tz);
                },__tx,__tz);
            }
        }
        for(auto&t:threads){
            t.join();
        }

        auto interpolate = [&](int x, int z, int type) {
            int x0 = x / SCALE_FACTOR;
            int z0 = z / SCALE_FACTOR;
            double h00, h01, h10, h11;
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

            double dx = (x % SCALE_FACTOR) / (float) SCALE_FACTOR;
            double dz = (z % SCALE_FACTOR) / (float) SCALE_FACTOR;

            double hx0 = utils::noise::lerp(h00, h10, dx);
            double hx1 = utils::noise::lerp(h01, h11, dx);

            double hf = utils::noise::lerp(hx0, hx1, dz);
            return hf;
        };

        ::world::gen::simple_grass_surface surface(seed);
        ::world::gen::simple_carver carver(seed);

        for (int x = 0; x < WORLD_SIZE * 16; x++) {
            for (int z = 0; z < WORLD_SIZE * 16; z++) {

//                int h=(int)interpolate(x,z,0);
                double elev = interpolate(x, z, 0);
                double rough = interpolate(x, z, 1);
                double detail = interpolate(x, z, 2);
//                    elev = 2 * elev - 1;
//                    rough = 2 * rough - 1;
//                    detail = 2 * detail - 1;
//                int h = (int) ((elev + rough * detail) * 64 + 64);
                int h=(int)(elev*64+64);
                if(h<0)h=0;
                if(h>=256)h=256;

                // TODO: better height calc
//                if (h < 10)h = 10;
//                if (h > 100)h = 100;

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