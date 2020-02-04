//
// Created by Jack Armstrong on 2/3/20.
//

#include "box_renderer.h"

//TODO rename from wireframe to box shader
extern "C" const unsigned char SHADER_wireframe_frag[];
extern "C" const size_t SHADER_wireframe_frag_len;

extern "C" const unsigned char SHADER_wireframe_vert[];
extern "C" const size_t SHADER_wireframe_vert_len;

namespace client {

    box_renderer::box_renderer() {
        shader = new gl::shader(SHADER_wireframe_vert, SHADER_wireframe_vert_len, SHADER_wireframe_frag,
                                SHADER_wireframe_frag_len);
        {
            gl::mesh_data data{
                    {{3, {
                                 0, 0, 0, 1, 0, 0,
                                 0, 0, 0, 0, 1, 0,
                                 0, 0, 0, 0, 0, 1,

                                 1, 0, 0, 1, 1, 0,
                                 1, 0, 0, 1, 0, 1,

                                 0, 1, 0, 1, 1, 0,
                                 0, 1, 0, 0, 1, 1,

                                 0, 0, 1, 1, 0, 1,
                                 0, 0, 1, 0, 1, 1,

                                 0, 1, 1, 1, 1, 1,
                                 1, 0, 1, 1, 1, 1,
                                 1, 1, 0, 1, 1, 1
                         }}},
                    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}
            };
            for (int i = 0; i < data.buffers[0].data.size(); i += 3) {
                data.buffers[0].data[i + 0] -= 0.5;
                data.buffers[0].data[i + 2] -= 0.5;
            }
            lines = new gl::mesh(&data);
        }
        {
            gl::mesh_data data{
                    {{3, {
                                 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
                                 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1,

                                 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1,
                                 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1,

                                 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,
                                 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1,
                         }}},
                    {
                     0, 1, 2, 1, 2, 3,
                            4, 5, 6, 5, 6, 7,
                            8, 9, 10, 9, 10, 11,
                            12, 13, 14, 13, 14, 15,
                            16, 17, 18, 17, 18, 19,
                            20, 21, 22, 21, 22, 23
                    }
            };
            for (int i = 0; i < data.buffers[0].data.size(); i += 3) {
                data.buffers[0].data[i + 0] -= 0.5;
                data.buffers[0].data[i + 2] -= 0.5;
            }
            fills = new gl::mesh(&data);
        }
    }

    box_renderer::~box_renderer() {
        delete shader;
        delete lines;
        delete fills;
    }

    void box_renderer::render_lines(client::ctx3d ctx, utils::aabb aabb) {
        shader->bind();
        shader->uniform4x4("perspective", ctx.perspective);
        shader->uniform4x4("view", ctx.view);
        shader->uniform4x4("model", glm::translate(glm::mat4(1), aabb.pos) * glm::scale(glm::mat4(1), aabb.size));
        lines->render_lines();
    }

    void box_renderer::render_fills(client::ctx3d ctx, utils::aabb aabb) {
        shader->bind();
        shader->uniform4x4("perspective", ctx.perspective);
        shader->uniform4x4("view", ctx.view);
        shader->uniform4x4("model", glm::translate(glm::mat4(1), aabb.pos) * glm::scale(glm::mat4(1), aabb.size));
        fills->render_triangles();
    }

}