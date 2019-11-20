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

    struct chunk {

        std::vector<block_state>palette;
        std::set<block_state>palette_set;
        unsigned short data[16][256][16]={{{0}}};//ushort is max 32767, assuming even no palette prunes it will *never* pass USHORT_MAX limit

        chunk();

        [[nodiscard]] bool in_bounds(int x,int y,int z)const;
        [[nodiscard]] bool in_bounds(glm::ivec3 v)const;
        [[nodiscard]] block_state get(int x, int y, int z)const;
        [[nodiscard]] block_state get(glm::ivec3 v)const;
        void set(int x, int y, int z, block_state b);
        void set(glm::ivec3 v, block_state b);

    };

    struct block_context {

        //xmi,xpl...
        block_state xmi,xpl,ymi,ypl,zmi,zpl;
        block_state self;
        glm::ivec3 pos;

    };

    struct block {

        int id;
        block_state defaultState;

        explicit block(int id)noexcept{
            this->id=id;
            this->defaultState=create(id,0);
        }

        virtual void render(gl::mesh_data*md, block_context ctx)const noexcept;

    };

    void render_full_block(gl::mesh_data*md, block_context ctx, client::quad_texture_descr xmi, client::quad_texture_descr xpl, client::quad_texture_descr ymi, client::quad_texture_descr ypl, client::quad_texture_descr zmi, client::quad_texture_descr zpl);


}

#endif //GLMINECRAFT3_BLOCK_H
