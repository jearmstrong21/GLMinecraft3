//
// Created by Jack Armstrong on 11/16/19.
//

#include "rendered_chunk.h"

namespace client {

    rendered_chunk::rendered_chunk(glm::ivec2 cpos) : cpos(cpos) {
        for (int y = 0; y < 16; y++) {
            section_data[y] = gl::mesh_data();
            block::client::init_chunk_buffers(&section_data[y]);
//            sections[y]=std::shared_ptr<gl::mesh>(new gl::mesh(&section_data[y]));
        }
    }

    void rendered_chunk::render_chunk(int section) {
        if (section == -1) {
            for (int y = 0; y < 16; y++) {
                render_chunk_section(y);
            }
        } else {
            render_chunk_section(section);
        }
    }

    void
    rendered_chunk::take_chunk(const block::world &world, const std::shared_ptr<block::chunk> &chunk, int section) {
        if (section == -1) {
            for (int y = 0; y < 16; y++) {
                take_chunk_section(world, chunk, y);
            }
        } else {
            take_chunk_section(world, chunk, section);
        }
    }

    void rendered_chunk::take_chunk_section(const block::world &world, const std::shared_ptr<block::chunk> &chunk,
                                            int section) {
        section_data[section] = gl::mesh_data();
        block::client::init_chunk_buffers(&section_data[section]);

        for (int x = 0; x < 16; x++) {
            for (int y = section * 16; y < section * 16 + 16; y++) {
                for (int z = 0; z < 16; z++) {
                    block::from_id(block::id(chunk->get(x, y, z)))->render(&section_data[section],
                                                                           world.get_block_context({cpos.x * 16 + x, y,
                                                                                                    cpos.y * 16 + z}));
                }
            }
        }
    }

    void rendered_chunk::render_chunk_section(int section) {
        sections[section] = std::shared_ptr<gl::mesh>(new gl::mesh(&section_data[section]));
    }

    void rendered_chunk::render(gl::shader *shader) {
        for (int y = 0; y < 16; y++) {
            shader->uniform4x4("model", glm::translate(glm::mat4(1), {0, 0, 0}));
            sections[y]->render_triangles();
        }
    }

}