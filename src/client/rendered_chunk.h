//
// Created by Jack Armstrong on 11/16/19.
//

#ifndef GLMINECRAFT3_RENDERED_CHUNK_H
#define GLMINECRAFT3_RENDERED_CHUNK_H

#include "block/world.h"
#include "gl/mesh.h"
#include "gl/meshdata.h"
#include "gl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace client {

    //Also will manage tile entity rendering and stuff
    struct RenderedChunk {
        gl::MeshData sectionData[16];
        std::shared_ptr<gl::Mesh> sections[16];
        glm::ivec2 cpos;

        explicit RenderedChunk(glm::ivec2 cpos);

        void takeChunk(const std::shared_ptr<block::World>&world,const std::shared_ptr<block::Chunk>&chunk,int section=-1);
        void render(const std::shared_ptr<gl::Shader>& shader);

    private:
        void takeChunkSection(const std::shared_ptr<block::World>&world,const std::shared_ptr<block::Chunk>&chunk,int section);

    };

}

#endif //GLMINECRAFT3_RENDERED_CHUNK_H
