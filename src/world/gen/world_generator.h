//
// Created by paladin on 12/29/19.
//

#ifndef GLMINECRAFT3_WORLD_GENERATOR_H
#define GLMINECRAFT3_WORLD_GENERATOR_H


//#include "world/world.h"

namespace block {
    class world;

    struct world_generator {
        void generate_world(world *);
    };
}


#endif //GLMINECRAFT3_WORLD_GENERATOR_H
