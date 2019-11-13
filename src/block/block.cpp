//
// Created by Jack Armstrong on 11/12/19.
//

#include "block.h"

namespace block {

    int id(BlockState bs){
        return (int)(bs>>32);
    }
    int meta(BlockState bs){
        return (int)bs;
    }
    BlockState create(int id,int meta){
        return (((long)id)<<32)|(meta&0xffffffffL);
    }

}