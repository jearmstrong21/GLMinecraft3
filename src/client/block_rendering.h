//
// Created by Jack Armstrong on 11/12/19.
//

#ifndef GLMINECRAFT3_BLOCK_RENDERING_H
#define GLMINECRAFT3_BLOCK_RENDERING_H

#include <glm/glm.hpp>
#include "utils/utils.h"
#include "gl/meshdata.h"

namespace block {

    namespace client {

#define ATLAS_TEXTURE(X, Y) (32*(31-Y)+(X))

        enum class atlas_texture {

            NONE = ATLAS_TEXTURE(31, 31),

            DIRT = ATLAS_TEXTURE(8, 6),
            GRASS_TOP = ATLAS_TEXTURE(15, 10),
            GRASS_SIDE = ATLAS_TEXTURE(12, 10),
            GRASS_SIDE_OVERLAY = ATLAS_TEXTURE(13, 10),
            STONE = ATLAS_TEXTURE(20, 9),

            BREAK_0 = ATLAS_TEXTURE(2, 7)

        };

        struct layer_texture_descr {

            atlas_texture texture = atlas_texture::NONE;
            glm::vec3 color = glm::vec3(1, 1, 1);

            glm::vec2 get_uv() {
                return glm::vec2((int) texture % 32, (int) texture / 32) / 32.0F;
            }

        };

        struct quad_texture_descr {

            layer_texture_descr first;
            layer_texture_descr second;

        };

        struct quad_descr {

            quad_texture_descr texture;
            glm::vec3 start;
            glm::vec3 d0;
            glm::vec3 d1;

            void xmi();

            void xpl();

            void ymi();

            void ypl();

            void zmi();

            void zpl();

        };

        void mesh_quad(gl::mesh_data *md, quad_descr qd, int x, int y, int z);

        void init_chunk_buffers(gl::mesh_data *md);

    }

}

#endif //GLMINECRAFT3_BLOCK_RENDERING_H
