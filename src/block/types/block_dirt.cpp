//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_dirt.h"

namespace block {


    BlockDirt::BlockDirt(int id)noexcept:Block(id) {

    }

    void BlockDirt::render(gl::MeshData *md, block::BlockContext ctx) const noexcept {
        renderFullBlock(md,ctx,side,side,side,side,side,side);
    }

}