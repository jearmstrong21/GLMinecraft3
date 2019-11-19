//
// Created by paladin on 11/19/19.
//

#ifndef GLMINECRAFT3_BASE_SURFACE_H
#define GLMINECRAFT3_BASE_SURFACE_H


#include <block/blockstate.h>

/**
 * SURFACES
 * In a Notchian server, surfaces (or surface builders) are responsible for filling in the noise-generated mesh with blocks
 * Examples:
 * + Sand/Sandstone/Stone/etc in a Desert
 * + Terracotta layers/Terracotta/Stone/etc in a Badlands
 */
namespace world::gen {
    class base_surface {
    public:
        explicit base_surface(int seed);
        block::BlockState getForLocation(int x, int y, int z, int h);
    };
}


#endif //GLMINECRAFT3_BASE_SURFACE_H
