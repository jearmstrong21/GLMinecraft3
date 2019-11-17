//
// Created by Jack Armstrong on 11/16/19.
//

#include "rendered_chunk.h"

namespace client {

    RenderedChunk::RenderedChunk(glm::ivec2 cpos): cpos(cpos) {
        for(int y=0;y<16;y++){
            sectionData[y]=gl::MeshData();
            block::client::initChunkBuffers(&sectionData[y]);
            sections[y]=std::shared_ptr<gl::Mesh>(new gl::Mesh(&sectionData[y]));
        }
    }

    void RenderedChunk::takeChunk(const std::shared_ptr<block::World>&world,const std::shared_ptr<block::Chunk>&chunk, int section) {
        if(section==-1){
            for(int y=0;y<16;y++){
                takeChunkSection(world,chunk,y);
            }
        }else{
            takeChunkSection(world,chunk,section);
        }
    }

    void RenderedChunk::takeChunkSection(const std::shared_ptr<block::World>&world,const std::shared_ptr<block::Chunk>&chunk, int section) {
        sectionData[section]=gl::MeshData();
        block::client::initChunkBuffers(&sectionData[section]);

        for(int x=0;x<16;x++){
            for(int y=section*16;y<section*16+16;y++){
                for(int z=0;z<16;z++){
                    block::fromID(block::id(chunk->get(x,y,z)))->render(&sectionData[section],world->getCTX({cpos.x*16+x,y,cpos.y*16+z}));
                }
            }
        }
        sections[section]=std::shared_ptr<gl::Mesh>(new gl::Mesh(&sectionData[section]));
    }

    void RenderedChunk::render(const std::shared_ptr<gl::Shader>& shader) {
        for(int y=0;y<16;y++) {
            shader->uniform4x4("model", glm::translate(glm::mat4(1), {0,0,0}));
            sections[y]->renderTriangles();
        }
    }

}