//
// Created by Jack Armstrong on 11/19/19.
//

#ifndef GLMINECRAFT3_SIMPLE_GRASS_SURFACE_H
#define GLMINECRAFT3_SIMPLE_GRASS_SURFACE_H

#include <cstdint>
#include "base_surface.h"
#include "block/blocks.h"

namespace world::gen {

    class simple_grass_surface: base_surface {
    public:
        explicit simple_grass_surface(uint32_t seed);
        [[nodiscard]] block::block_state get_for_location(int x,int y,int z,int h) override;
    };

}

#endif //GLMINECRAFT3_SIMPLE_GRASS_SURFACE_H
