//
// Created by Jack Armstrong on 1/30/20.
//

#include "item_renderer.h"

extern "C" const unsigned char SHADER_item_frag[];
extern "C" const size_t SHADER_item_frag_len;

extern "C" const unsigned char SHADER_item_vert[];

extern "C" const size_t SHADER_item_vert_len;

namespace client {

    void
    item_renderer::render_item(client::gui_ctx ctx, const client::item_texture_descr &i, gl::texture *texture, int x,
                               int y, int s) {
        shader->bind();
        shader->uniform4x4("ortho", ctx.ortho);
        shader->uniform2("pos", {x, y});
        shader->uniform2("size", {s, s});
        shader->texture("tex", texture, 0);
        for (layer_texture_descr l:i.layers) {
            shader->uniform2("uv_pos", l.get_uv());
            shader->uniform2("uv_size", {1.0F / 32.0F, 1.0F / 32.0F});
            shader->uniform3("tint", l.color);
            mesh->render_triangles();
        }
    }

    item_renderer::item_renderer() {
        shader = new gl::shader(SHADER_item_vert, SHADER_item_vert_len, SHADER_item_frag, SHADER_item_frag_len);
        gl::mesh_data data{
                {
                        {2, {0, 0, 1, 0, 0, 1, 1, 1}},
                },
                {       0, 1, 2, 1, 2, 3}
        };
        mesh = new gl::mesh(&data);
    }

    item_renderer::~item_renderer() {
        delete shader;
        delete mesh;
    }

    void
    item_renderer::render_texture(gui_ctx ctx, gl::texture *texture, glm::vec2 uv_pos, glm::vec2 uv_size,
                                  glm::vec3 tint, int x, int y,
                                  int w, int h) {
        shader->bind();
        shader->uniform4x4("ortho", ctx.ortho);
        shader->uniform2("pos", {x, y});
        shader->uniform2("size", {w, h});
        shader->texture("tex", texture, 0);
        shader->uniform2("uv_pos", uv_pos);
        shader->uniform2("uv_size", uv_size);
        shader->uniform3("tint", tint);
        mesh->render_triangles();
    }

    void
    item_renderer::render_item(const item_texture_descr &i, gl::shader *s, const std::function<void()> &render_layers) {
        for (layer_texture_descr l:i.layers) {
            s->uniform2("uv_pos", l.get_uv());
            s->uniform2("uv_size", {1.0F / 32.0F, 1.0F / 32.0F});
            s->uniform3("tint", l.color);
            render_layers();
        }
    }

}