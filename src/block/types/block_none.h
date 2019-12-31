//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_BLOCK_NONE_H
#define GLMINECRAFT3_BLOCK_NONE_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct block_none : block {

        explicit block_none(int id) noexcept;

        void render(gl::mesh_data *md, block_context ctx) const noexcept override;

    };

    const block_none NONE(BLOCK_ID_NONE);

}

#endif //GLMINECRAFT3_BLOCK_NONE_H
