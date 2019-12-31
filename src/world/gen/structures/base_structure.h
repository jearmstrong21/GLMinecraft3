//
// Created by paladin on 11/19/19.
//

#ifndef GLMINECRAFT3_BASE_STRUCTURE_H
#define GLMINECRAFT3_BASE_STRUCTURE_H


/**
 * STRUCTURES
 * In a Notchian server, structures are large-scale terrain-independent features.
 * Examples:
 * + Stronghold
 * + Village
 * + Desert Temple
 */
#include <cstdint>

namespace world::gen {
    class base_structure {
        uint32_t seed;
    public:
        explicit base_structure(uint32_t seed);

        virtual void perform() = 0;
    };
}


#endif //GLMINECRAFT3_BASE_STRUCTURE_H
