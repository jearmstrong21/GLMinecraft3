//
// Created by Jack Armstrong on 11/12/19.
//

#ifndef GLMINECRAFT3_BLOCK_RENDERING_H
#define GLMINECRAFT3_BLOCK_RENDERING_H

#include <glm/glm.hpp>
#include "utils/utils.h"
#include "gl/meshdata.h"

namespace client {

    namespace block {

#define ATLASTEXTURE(X,Y) (32*(31-Y)+(X))

        enum class AtlasTexture {

            NONE = ATLASTEXTURE(31,31),

            DIRT = ATLASTEXTURE(8,6),
            GRASS_TOP=ATLASTEXTURE(15,10),
            GRASS_SIDE=ATLASTEXTURE(12,10),
            GRASS_SIDE_OVERLAY=ATLASTEXTURE(13,10),

            BREAK_0=ATLASTEXTURE(2,7)

        };

        struct LayerTextureDescr {

            AtlasTexture texture=AtlasTexture::NONE;
            glm::vec3 color=glm::vec3(1,1,1);

            glm::vec2 getUV(){
                return glm::vec2((int)texture%32,(int)texture/32)/32.0F;
            }

        };

        struct QuadTextureDescr {

            LayerTextureDescr first;
            LayerTextureDescr second;

        };

        struct QuadDescr {

            QuadTextureDescr texture;
            glm::vec3 start;
            glm::vec3 d0;
            glm::vec3 d1;

        };

        void meshQuad(gl::MeshData*md,QuadDescr qd,int x,int y,int z);
        void initChunkBuffers(gl::MeshData*md);


    }

}

#endif //GLMINECRAFT3_BLOCK_RENDERING_H
