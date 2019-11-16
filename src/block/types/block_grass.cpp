//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_grass.h"

namespace block {


    BlockGrass::BlockGrass(int id)noexcept:Block(id) {

    }

    void BlockGrass::render(gl::MeshData *md, block::BlockContext *ctx) const noexcept {
        renderFullBlock(md,ctx,side,side,bot,top,side,side);
    }

}