//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_BLOCKSTATE_H
#define GLMINECRAFT3_BLOCKSTATE_H

namespace block {

    typedef long block_state;

    int id(block_state bs);

    int meta(block_state bs);

    block_state create(int id, int meta);

}

#endif //GLMINECRAFT3_BLOCKSTATE_H
