//
// Created by Jack Armstrong on 11/12/19.
//

#ifndef GLMINECRAFT3_BLOCK_H
#define GLMINECRAFT3_BLOCK_H

namespace block {

    typedef long BlockState;

    int id(BlockState bs);
    int meta(BlockState bs);
    BlockState create(int id,int meta=0);

}

#endif //GLMINECRAFT3_BLOCK_H
