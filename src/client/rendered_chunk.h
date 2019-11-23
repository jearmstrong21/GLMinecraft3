//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_RENDERED_CHUNK_H
#define GLMINECRAFT3_RENDERED_CHUNK_H

#include "world/world.h"
#include "gl/mesh.h"
#include "gl/meshdata.h"
#include "gl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace client {

    //Also will manage tile entity rendering and stuff
    struct rendered_chunk {
        gl::mesh_data section_data[16];
        std::shared_ptr<gl::mesh> sections[16];
        glm::ivec2 cpos;

        explicit rendered_chunk(glm::ivec2 cpos);

        void take_chunk(const std::shared_ptr<block::world>&world, const std::shared_ptr<block::chunk>&chunk, int section= -1);
        void render_chunk(int section=-1);
        void render(const std::shared_ptr<gl::shader>& shader);

    private:
        void take_chunk_section(const std::shared_ptr<block::world>&world, const std::shared_ptr<block::chunk>&chunk, int section);
        void render_chunk_section(int section);

    };

}

#endif //GLMINECRAFT3_RENDERED_CHUNK_H
