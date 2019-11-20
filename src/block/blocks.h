//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_BLOCKS_H
#define GLMINECRAFT3_BLOCKS_H

#include "block.h"
#include "block/types/block_none.h"
#include "block/types/block_grass.h"
#include "block/types/block_dirt.h"
#include "block/types/block_stone.h"

namespace block {

    const block* from_id(int id);

}

#endif //GLMINECRAFT3_BLOCKS_H
