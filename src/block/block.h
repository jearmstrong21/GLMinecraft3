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

namespace block {

    struct Chunk {

        std::vector<BlockState>palette;
        std::set<BlockState>paletteSet;
        unsigned short data[16][256][16]={{{0}}};//ushort is max 32767, assuming even no palette prunes it will *never* pass USHORT_MAX limit

        Chunk(){
            palette.push_back(0);
            paletteSet.insert(0);
        }

        [[nodiscard]] bool in_bounds(int x,int y,int z)const{
            return x>=0&&y>=0&&z>=0&&x<16&&y<256&&z<16;
        }
        [[nodiscard]] bool in_bounds(glm::ivec3 v)const{return in_bounds(v.x,v.y,v.z);}
        [[nodiscard]] BlockState get(int x,int y,int z)const{
            if(in_bounds(x,y,z))return palette[data[x][y][z]];
            return 0;
        }
        [[nodiscard]] BlockState get(glm::ivec3 v)const{return get(v.x,v.y,v.z);}
        void set(int x,int y,int z,BlockState b){
            if(in_bounds(x,y,z)){
                printf("SET %i,%i,%i: %i\n",x,y,z,b);
                if(paletteSet.count(b)==0){
                    palette.push_back(b);
                    paletteSet.insert(b);
                    data[x][y][z]=palette.size()-1;
                }else{
                    data[x][y][z]=std::distance(palette.begin(),std::find(palette.begin(),palette.end(),b));
                }
            }
        }
        void set(glm::ivec3 v,BlockState b){set(v.x,v.y,v.z,b);}

    };

    struct BlockContext {

        //xmi,xpl...
        BlockState xmi,xpl,ymi,ypl,zmi,zpl;
        BlockState self;
        glm::ivec3 pos;

    };

    struct Block {

        int id;

        explicit Block(int id)noexcept{
            this->id=id;
        }

        virtual void render(gl::MeshData*md,BlockContext*ctx)const noexcept=0;
        [[nodiscard]] BlockState getDefaultState()const noexcept{
            return create(id,0);
        }

    };

    void renderFullBlock(gl::MeshData*md,BlockContext*ctx,client::QuadTextureDescr xmi,client::QuadTextureDescr xpl,client::QuadTextureDescr ymi,client::QuadTextureDescr ypl,client::QuadTextureDescr zmi,client::QuadTextureDescr zpl);


}

#endif //GLMINECRAFT3_BLOCK_H
