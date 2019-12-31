//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_BLOCK_STONE_H
#define GLMINECRAFT3_BLOCK_STONE_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct block_stone : block {

        explicit block_stone(int id) noexcept;

        const client::quad_texture_descr side{{client::atlas_texture::STONE, glm::vec3(1, 1, 1)},
                                              {client::atlas_texture::NONE,  glm::vec3(1, 1, 1)}};

        void render(gl::mesh_data *md, block_context ctx) const noexcept override;

    };

    const block_stone STONE(BLOCK_ID_STONE);

}

#endif //GLMINECRAFT3_BLOCK_STONE_H
