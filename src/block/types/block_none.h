//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_BLOCK_NONE_H
#define GLMINECRAFT3_BLOCK_NONE_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct BlockNone: Block {

        explicit BlockNone(int id)noexcept;

        void render(gl::MeshData*md,BlockContext ctx) const noexcept;

    };

    const BlockNone NONE(BLOCK_ID_NONE);

}

#endif //GLMINECRAFT3_BLOCK_NONE_H
