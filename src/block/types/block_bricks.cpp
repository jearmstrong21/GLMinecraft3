//
// Created by Jack Armstrong on 2/9/20.
//

#include "block_bricks.h"

namespace block {


    block_bricks::block_bricks(int id) noexcept: block(id) {

    }

    void block_bricks::render(gl::mesh_data *md, block_context ctx) const noexcept {
        render_full_block(md, ctx, side, side, side, side, side, side);
    }

}