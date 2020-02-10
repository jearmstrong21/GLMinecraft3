//
// Created by Jack Armstrong on 2/9/20.
//

#ifndef GLMINECRAFT3_BLOCK_BRICKS_H
#define GLMINECRAFT3_BLOCK_BRICKS_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct block_bricks : block {

        explicit block_bricks(int id) noexcept;

        const client::quad_texture_descr side{{client::atlas_texture::BRICKS, glm::vec3(1, 1, 1)},
                                              {client::atlas_texture::NONE,   glm::vec3(1, 1, 1)}};

        void render(gl::mesh_data *md, block_context ctx) const noexcept override;

    };

    const block_bricks BRICKS(BLOCK_ID_BRICKS);

}

#endif //GLMINECRAFT3_BLOCK_BRICKS_H
