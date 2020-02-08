//
// Created by Jack Armstrong on 1/30/20.
//

#ifndef GLMINECRAFT3_ITEM_RENDERER_H
#define GLMINECRAFT3_ITEM_RENDERER_H

#include "rendering.h"
#include <vector>
#include <gl/shader.h>
#include <gl/mesh.h>
#include "gui_ctx.h"
#include <functional>

namespace client {

    struct item_texture_descr {

        std::vector<layer_texture_descr> layers;
        int count;
        //int durability; text::formatted name;

    };

    struct item_renderer {

        gl::shader *shader;
        gl::mesh *mesh;

        item_renderer();

        ~item_renderer();

        void render_item(gui_ctx ctx, const item_texture_descr &i, gl::texture *texture, int x, int y, int s);

        void render_item(const item_texture_descr &i, gl::shader *s, const std::function<void()> &render_layer);

        void
        render_texture(gui_ctx ctx, gl::texture *texture, glm::vec2 uv_pos, glm::vec2 uv_size, glm::vec3 tint, int x,
                       int y,
                       int w, int h);

    };

}

#endif //GLMINECRAFT3_ITEM_RENDERER_H
