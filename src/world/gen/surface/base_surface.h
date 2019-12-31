//
// Created by paladin on 11/19/19.
//

#ifndef GLMINECRAFT3_BASE_SURFACE_H
#define GLMINECRAFT3_BASE_SURFACE_H


#include <cstdint>
#include "block/blockstate.h"

/**
 * SURFACES
 * In a Notchian server, surfaces (or surface builders) are responsible for filling in the noise-generated mesh with blocks
 * Examples:
 * + Sand/Sandstone/Stone/etc in a Desert
 * + Terracotta layers/Terracotta/Stone/etc in a Badlands
 */
namespace world::gen {
    class base_surface {
    private:
        uint32_t seed;
    public:
        explicit base_surface(uint32_t seed);

        [[nodiscard]] virtual block::block_state get_for_location(int x, int y, int z, int h) = 0;
    };
}


#endif //GLMINECRAFT3_BASE_SURFACE_H
