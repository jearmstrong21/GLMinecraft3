//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_BLOCK_DIRT_H
#define GLMINECRAFT3_BLOCK_DIRT_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct BlockDirt: Block {

        explicit BlockDirt(int id)noexcept;

        const client::QuadTextureDescr side{{client::AtlasTexture::DIRT,glm::vec3(1,1,1)},{client::AtlasTexture::NONE,glm::vec3(1,1,1)}};

        void render(gl::MeshData*md,BlockContext ctx) const noexcept;

    };

    const BlockDirt DIRT(BLOCK_ID_DIRT);

}

#endif //GLMINECRAFT3_BLOCK_DIRT_H
