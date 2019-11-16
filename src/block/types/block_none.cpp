//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_none.h"

namespace block {

    BlockNone::BlockNone(int id) noexcept: Block(id) {

    }

    void BlockNone::render(gl::MeshData *md, block::BlockContext ctx) const noexcept {

    }

}