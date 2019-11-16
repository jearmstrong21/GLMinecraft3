//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_BLOCK_STONE_H
#define GLMINECRAFT3_BLOCK_STONE_H

#include "block/block.h"
#include "block/block_ids.h"

namespace block {

    struct BlockStone: Block {

        explicit BlockStone(int id)noexcept;

        const client::QuadTextureDescr side{{client::AtlasTexture::STONE,glm::vec3(1,1,1)},{client::AtlasTexture::NONE,glm::vec3(1,1,1)}};

        void render(gl::MeshData*md,BlockContext ctx) const noexcept;

    };

    const BlockStone STONE(BLOCK_ID_STONE);

}

#endif //GLMINECRAFT3_BLOCK_STONE_H
