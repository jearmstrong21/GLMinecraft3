//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_stone.h"

namespace block {


    BlockStone::BlockStone(int id)noexcept:Block(id) {

    }

    void BlockStone::render(gl::MeshData *md, block::BlockContext *ctx) const noexcept {
        renderFullBlock(md,ctx,side,side,side,side,side,side);
    }

}