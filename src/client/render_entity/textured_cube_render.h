//
// Created by Jack Armstrong on 12/31/19.
//

#ifndef GLMINECRAFT3_TEXTURED_CUBE_RENDER_H
#define GLMINECRAFT3_TEXTURED_CUBE_RENDER_H

#include "gl/mesh.h"
#include "gl/shader.h"
#include <glm/gtc/matrix_transform.hpp>

extern "C" const unsigned char SHADER_entity_frag[];
extern "C" const size_t SHADER_entity_frag_len;

extern "C" const unsigned char SHADER_entity_vert[];
extern "C" const size_t SHADER_entity_vert_len;

namespace client {

    struct textured_cube_renderer {
        gl::mesh *mesh;
        gl::shader *shader;

        textured_cube_renderer() {

            gl::mesh_data data;
            mesh = new gl::mesh(&data);

            shader = new gl::shader(SHADER_entity_vert, SHADER_entity_vert_len, SHADER_entity_frag,
                                    SHADER_entity_frag_len);
        }

        void
        render_cube(glm::mat4 perspective, glm::mat4 view, glm::mat4 model, gl::texture *texture, glm::vec3 boxSize,
                    glm::vec2 textureOffset, glm::vec2 textureSize) {
            shader->bind();
            shader->uniform4x4("perspective", perspective);
            shader->uniform4x4("view", view);
            shader->uniform4x4("model", model * glm::scale(glm::mat4(1), glm::vec3{1.0F / 16.0F}));
            shader->texture("tex", texture, 0);
            float tx = textureOffset.x / textureSize.x;
            float ty = textureOffset.y / textureSize.y;
            float dx = boxSize.x / textureSize.x;
            float dy = boxSize.y / textureSize.y;
            float dz = boxSize.z / textureSize.y;
            gl::mesh_data data{
                    {
                            {3, {
                                        0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,//xmi
                                        1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1,//xpl

                                        0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1,//ymi
                                        0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1,//ypl

                                        0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0,//zmi
                                        0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1,//zpl
                                }},
                               {2, {
                                           tx + dx + dz + dz, ty, tx + dx + dz + dz, ty + dy, tx + dx + dz, ty,
                                           tx + dx + dz, ty + dy,//xmi
                                           tx, ty, tx, ty + dy, tx + dz, ty, tx + dz, ty + dy,//xpl
                                           tx + dz + dx, ty + dy, tx + dz + dx + dx, ty + dy, tx + dz + dx,
                                           ty + dy + dz, tx + dz + dx + dx, ty + dy + dz,//ymi
                                           tx + dz, ty + dy, tx + dz + dx, ty + dy, tx + dz, ty + dy + dz, tx + dz + dx,
                                           ty + dy + dz,//ypl
                                           tx + dz + dx + dz, ty, tx + dz + dx + dx + dz, ty, tx + dz + dx + dz,
                                           ty + dy, tx + dz + dx + dx + dz, ty + dy,//zmi
                                           tx + dz + dx, ty, tx + dz, ty, tx + dz + dx, ty + dy, tx + dz, ty + dy,//zpl
                                   }}
                    },//TODO: flip zpl and the right

                    {
                            0, 1, 2, 1, 2, 3,
                            4, 5, 6, 5, 6, 7,

                            8, 9, 10, 9, 10, 11,
                            12, 13, 14, 13, 14, 15,
                            16, 17, 18, 17, 18, 19,
                            20, 21, 22, 21, 22, 23}
            };
            for (int i = 0; i < data.buffers[0].data.size(); i += 3) {
                data.buffers[0].data[i + 0] *= boxSize.x;
                data.buffers[0].data[i + 1] *= boxSize.y;
                data.buffers[0].data[i + 2] *= boxSize.z;
                data.buffers[0].data[i + 0] -= boxSize.x / 2.0F;
//                data.buffers[0].data[i + 1] -= boxSize.y / 2.0F;
                data.buffers[0].data[i + 2] -= boxSize.z / 2.0F;
            }
            delete mesh;
            mesh = new gl::mesh(&data);
            mesh->render_triangles();
        }

        ~textured_cube_renderer() {
            delete mesh;
            delete shader;
        }
    };

}

#endif //GLMINECRAFT3_TEXTURED_CUBE_RENDER_H
