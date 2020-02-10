//
// Created by Jack Armstrong on 11/16/19.
//

#include "blocks.h"

namespace block {

    const block *from_id(int id) {

        switch (id) {
            case BLOCK_ID_GRASS:
                return &GRASS;
            case BLOCK_ID_DIRT:
                return &DIRT;
            case BLOCK_ID_STONE:
                return &STONE;
            case BLOCK_ID_BRICKS:
                return &BRICKS;
        }

//        printf("le wot %i\n", uuid);
//        std::exit(11);
        return &NONE;

    }

}