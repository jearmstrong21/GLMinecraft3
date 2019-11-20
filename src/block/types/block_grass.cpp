//
// Created by Jack Armstrong on 11/16/19.
//

#include "block_grass.h"

namespace block {


    block_grass::block_grass(int id)noexcept: block(id) {

    }

    void block_grass::render(gl::mesh_data*md, block_context ctx) const noexcept {
        render_full_block(md, ctx, side, side, bot, top, side, side);
    }

}