//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_dirt.h"

namespace block {


    block_dirt::block_dirt(int id) noexcept: block(id) {

    }

    void block_dirt::render(gl::mesh_data *md, block_context ctx) const noexcept {
        render_full_block(md, ctx, side, side, side, side, side, side);
    }

}