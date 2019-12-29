//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_BLOCK_DIRT_H
#define GLMINECRAFT3_BLOCK_DIRT_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct block_dirt: block {

        explicit block_dirt(int id)noexcept;

        const client::quad_texture_descr side{{client::atlas_texture::DIRT, glm::vec3(1, 1, 1)}, {client::atlas_texture::NONE, glm::vec3(1, 1, 1)}};

        void render(gl::mesh_data*md, block_context ctx) const noexcept override;

    };

    const block_dirt DIRT(BLOCK_ID_DIRT);

}

#endif //GLMINECRAFT3_BLOCK_DIRT_H
