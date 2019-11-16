//
// Created by Jack Armstrong on 11/15/19.
//

#include "blockstate.h"

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