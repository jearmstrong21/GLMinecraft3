//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_stone.h"

namespace block {


    block_stone::block_stone(int id)noexcept: block(id) {

    }

    void block_stone::render(gl::mesh_data *md, block_context ctx) const noexcept {
        render_full_block(md, ctx, side, side, side, side, side, side);
    }

}