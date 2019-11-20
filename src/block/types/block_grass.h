//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_BLOCK_GRASS_H
#define GLMINECRAFT3_BLOCK_GRASS_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct block_grass: block {

        explicit block_grass(int id)noexcept;

        const client::quad_texture_descr top{{client::atlas_texture::GRASS_TOP, glm::vec3(0.47, 0.82, 0.37)}, {client::atlas_texture::NONE, glm::vec3(1, 1, 1)}};
        const client::quad_texture_descr side{{client::atlas_texture::GRASS_SIDE, glm::vec3(1, 1, 1)}, {client::atlas_texture::GRASS_SIDE_OVERLAY, glm::vec3(0.47, 0.82, 0.37)}};
        const client::quad_texture_descr bot{{client::atlas_texture::DIRT, glm::vec3(1, 1, 1)}, {client::atlas_texture::NONE, glm::vec3(1, 1, 1)}};

        void render(gl::mesh_data*md, block_context ctx) const noexcept override;

    };

    const block_grass GRASS(BLOCK_ID_GRASS);

}

#endif //GLMINECRAFT3_BLOCK_GRASS_H
