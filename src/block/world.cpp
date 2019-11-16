//
// Created by Jack Armstrong on 11/16/19.
//

#include "world.h"

namespace block {
    World::World() {
//        for (int x = 0; x < WORLD_SIZE; x++) {
//            for (int z = 0; z < WORLD_SIZE; z++) {
//                for (int a = 0; a < 16; a++) {
//                    int h = x + z;
//                    for (int b = 0; b < 16; b++) {
//                        for (int y = 0; y <= h; y++) {
//                            BlockState bs=0;
//                            if(y<h-5)b=block::STONE.getDefaultState();
//                            else if(y<h)b=block::DIRT.getDefaultState();
//                            else if(y==h)b=block::GRASS.getDefaultState();
//                            map[x][z].set(a,y,b,bs);
//                        }
//                    }
//                }
//            }
//        }
    }

    bool World::in_bounds(int x, int y, int z) const{
        return x >= 0 && y >= 0 && z >= 0 && x < WORLD_SIZE * 16 && y < 256 && z < WORLD_SIZE * 16;
    }

    bool World::in_bounds(glm::ivec3 v) const{ return in_bounds(v.x, v.y, v.z); };

    BlockState World::get(int x, int y, int z) const{
        if (in_bounds(x, y, z))return map[x / 16][z / 16].get(x % 16, y, z % 16);
        return 0;
    }

    BlockState World::get(glm::ivec3 v) const{ return get(v.x, v.y, v.z); }

    void World::set(int x, int y, int z, BlockState b) {
        if (in_bounds(x, y, z))map[x / 16][z / 16].set(x % 16, y, z % 16, b);
    }

    void World::set(glm::ivec3 v, BlockState b) { set(v.x, v.y, v.z, b); }

    BlockContext World::getCTX(glm::ivec3 p) const {
        return BlockContext{get(p.x-1,p.y,p.z),get(p.x+1,p.y,p.z),get(p.x,p.y-1,p.z),get(p.x,p.y+1,p.z),get(p.x,p.y,p.z-1),get(p.x,p.y,p.z+1),get(p),p};
    }

}