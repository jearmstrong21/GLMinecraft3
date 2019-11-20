//
// Created by paladin on 11/19/19.
//

#ifndef GLMINECRAFT3_BASE_FEATURE_H
#define GLMINECRAFT3_BASE_FEATURE_H

/**
 * FEATURES
 * In a Notchian server, features are responsible for *small* terrain-specific decorators
 * Examples:
 * + Trees
 * + Flowers
 * + Cacti
 * + Ores
 */
#include <cstdint>

namespace world::gen {
    class base_feature {
        uint32_t seed;
    public:
        explicit base_feature(uint32_t seed);
        virtual void build()=0;
        // TODO: add argument (we need to pass set function from world)
    };
}


#endif //GLMINECRAFT3_BASE_FEATURE_H
