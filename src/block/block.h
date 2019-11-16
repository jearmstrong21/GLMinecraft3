//
// Created by Jack Armstrong on 11/12/19.
//

#ifndef GLMINECRAFT3_BLOCK_H
#define GLMINECRAFT3_BLOCK_H

#include <vector>
#include <glm/glm.hpp>
#include <map>
#include <set>
#include <gl/meshdata.h>
#include "client/block_rendering.h"
#include "blockstate.h"
#include <csignal>

namespace block {

    struct Chunk {

        std::vector<BlockState>palette;
        std::set<BlockState>paletteSet;
        unsigned short data[16][256][16]={{{0}}};//ushort is max 32767, assuming even no palette prunes it will *never* pass USHORT_MAX limit

        Chunk();

        [[nodiscard]] bool in_bounds(int x,int y,int z)const;
        [[nodiscard]] bool in_bounds(glm::ivec3 v)const;
        [[nodiscard]] BlockState get(int x,int y,int z)const;
        [[nodiscard]] BlockState get(glm::ivec3 v)const;
        void set(int x,int y,int z,BlockState b);
        void set(glm::ivec3 v,BlockState b);

    };

    struct BlockContext {

        //xmi,xpl...
        BlockState xmi,xpl,ymi,ypl,zmi,zpl;
        BlockState self;
        glm::ivec3 pos;

    };

    struct Block {

        int id;
        BlockState defaultState;

        explicit Block(int id)noexcept{
            this->id=id;
            this->defaultState=create(id,0);
        }

        virtual void render(gl::MeshData*md,BlockContext ctx)const noexcept;

    };

    void renderFullBlock(gl::MeshData*md,BlockContext ctx,client::QuadTextureDescr xmi,client::QuadTextureDescr xpl,client::QuadTextureDescr ymi,client::QuadTextureDescr ypl,client::QuadTextureDescr zmi,client::QuadTextureDescr zpl);


}

#endif //GLMINECRAFT3_BLOCK_H
