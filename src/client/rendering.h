//
// Created by Jack Armstrong on 1/30/20.
//

#ifndef GLMINECRAFT3_RENDERING_H
#define GLMINECRAFT3_RENDERING_H

#include <glm/glm.hpp>

namespace client {

#define ATLAS_TEXTURE(X, Y) (32*(31-Y)+(X))

    enum class atlas_texture {

        NONE = ATLAS_TEXTURE(31, 31),

        DIRT = ATLAS_TEXTURE(8, 6),
        GRASS_TOP = ATLAS_TEXTURE(15, 10),
        GRASS_SIDE = ATLAS_TEXTURE(12, 10),
        GRASS_SIDE_OVERLAY = ATLAS_TEXTURE(13, 10),
        STONE = ATLAS_TEXTURE(20, 9),
        BRICKS = ATLAS_TEXTURE(5, 3),

        DIAMOND_SWORD = ATLAS_TEXTURE(27, 1),
        GOLD_SWORD = ATLAS_TEXTURE(30, 15),
        IRON_SWORD = ATLAS_TEXTURE(31, 12),
        STONE_SWORD = ATLAS_TEXTURE(15, 18),
        WOOD_SWORD = ATLAS_TEXTURE(23, 18),

        UNKNOWN = ATLAS_TEXTURE(25, 18),

    };

    struct layer_texture_descr {

        atlas_texture texture = atlas_texture::NONE;
        glm::vec3 color = glm::vec3(1, 1, 1);

        glm::vec2 get_uv() {
            return glm::vec2((int) texture % 32, (int) texture / 32) / 32.0F;
        }

    };

}

#endif //GLMINECRAFT3_RENDERING_H
