//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_BLOCKSTATE_H
#define GLMINECRAFT3_BLOCKSTATE_H

namespace block {

    typedef long BlockState;

    int id(BlockState bs);
    int meta(BlockState bs);
    BlockState create(int id,int meta);

}

#endif //GLMINECRAFT3_BLOCKSTATE_H
