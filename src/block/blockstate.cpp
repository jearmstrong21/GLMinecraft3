//
// Created by Jack Armstrong on 11/15/19.
//

#include "blockstate.h"

namespace block {

    int id(block_state bs) {
        return (int) (bs >> 32);
    }

    int meta(block_state bs) {
        return (int) bs;
    }

    block_state create(int id, int meta) {
        return (((long) id) << 32) | (meta & 0xffffffffL);
    }

}