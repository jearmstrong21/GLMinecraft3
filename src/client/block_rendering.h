//
// Created by Jack Armstrong on 11/12/19.
//

#ifndef GLMINECRAFT3_BLOCK_RENDERING_H
#define GLMINECRAFT3_BLOCK_RENDERING_H

#include <glm/glm.hpp>
#include "utils/utils.h"
#include "gl/meshdata.h"
#include "rendering.h"

namespace client {

    struct quad_texture_descr {

        layer_texture_descr first;
        layer_texture_descr second;

    };

    struct quad_descr {

        quad_texture_descr texture;
        glm::vec3 start;
        glm::vec3 d0;
        glm::vec3 d1;
        float ao[4] = {1, 1, 1, 1};

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

#endif //GLMINECRAFT3_BLOCK_RENDERING_H
