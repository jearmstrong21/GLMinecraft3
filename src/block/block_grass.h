//
// Created by Jack Armstrong on 11/15/19.
//

#ifndef GLMINECRAFT3_BLOCK_GRASS_H
#define GLMINECRAFT3_BLOCK_GRASS_H

#include "block.h"
#include "block_ids.h"

namespace block {

    struct BlockGrass: Block {

        explicit BlockGrass(int id):Block(id){};

        const client::QuadTextureDescr top{{client::AtlasTexture::GRASS_TOP,glm::vec3(0.47,0.82,0.37)},{client::AtlasTexture::NONE,glm::vec3(1,1,1)}};
        const client::QuadTextureDescr side{{client::AtlasTexture::GRASS_SIDE,glm::vec3(1,1,1)},{client::AtlasTexture::GRASS_SIDE_OVERLAY,glm::vec3(0.47,0.82,0.37)}};
        const client::QuadTextureDescr bot{{client::AtlasTexture::DIRT,glm::vec3(1,1,1)},{client::AtlasTexture::NONE,glm::vec3(1,1,1)}};

        void render(gl::MeshData*md,BlockContext*ctx) const override {
            renderFullBlock(md,ctx,side,side,bot,top,side,side);
        }

    };

    const BlockGrass GRASS(BLOCK_ID_GRASS);

}

#endif //GLMINECRAFT3_BLOCK_GRASS_H
