//
// Created by Jack Armstrong on 2/3/20.
//

#ifndef GLMINECRAFT3_BOX_RENDERER_H
#define GLMINECRAFT3_BOX_RENDERER_H

#include "gl/shader.h"
#include "gl/mesh.h"
#include "ctx3d.h"
#include "utils/aabb.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace client {

    struct box_renderer {

        gl::shader *shader;
        gl::mesh *lines;
        gl::mesh *fills;

        box_renderer();

        ~box_renderer();

        void render_fills(ctx3d ctx, utils::aabb aabb);

        void render_lines(ctx3d ctx, utils::aabb aabb);

    };

}

#endif //GLMINECRAFT3_BOX_RENDERER_H
