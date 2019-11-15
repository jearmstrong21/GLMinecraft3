//
// Created by Jack Armstrong on 11/12/19.
//

#ifndef GLMINECRAFT3_BLOCK_H
#define GLMINECRAFT3_BLOCK_H

#include <vector>
#include <glm/glm.hpp>
#include <map>
#include <set>

namespace block {

    typedef long BlockState;

    int id(BlockState bs){
        return (int)(bs>>32);
    }
    int meta(BlockState bs){
        return (int)bs;
    }
    BlockState create(int id,int meta){
        return (((long)id)<<32)|(meta&0xffffffffL);
    }


    struct Chunk {

        std::vector<BlockState>palette;
        std::set<BlockState>paletteSet;
        unsigned short data[16][256][16];//ushort is max 32767, assuming even no palette prunes it will *never* pass USHORT_MAX limit

        Chunk(){
            palette.push_back(0);
            paletteSet.insert(0);
        }

        bool in_bounds(int x,int y,int z){
            return x>=0&&y>=0&&z>=0&&x<16&&y<256&&z<16;
        }
        bool in_bounds(glm::ivec3 v){return in_bounds(v.x,v.y,v.z);}
        BlockState get(int x,int y,int z){
            if(in_bounds(x,y,z))return palette[data[x][y][z]];
            return 0;
        }
        BlockState get(glm::ivec3 v){return get(v.x,v.y,v.z);}
        void set(int x,int y,int z,BlockState b){
            if(in_bounds(x,y,z)){
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

        void take_palette(BlockState b){
            palette.push_back(b);
            paletteSet.insert(b);
        }

    };

#define WORLD_SIZE 16

    struct World {

        Chunk map[WORLD_SIZE][WORLD_SIZE];

        World();

        bool in_bounds(int x,int y,int z){
            return x>=0&&y>=0&&z>=0&&x<WORLD_SIZE*16&&y<256&&z<WORLD_SIZE*16;
        }
        bool in_bounds(glm::ivec3 v){return in_bounds(v.x,v.y,v.z);};
        BlockState get(int x,int y,int z){
            if(in_bounds(x,y,z))return map[x/16][z/16].get(x%16,y,z%16);
            return 0;
        }
        BlockState get(glm::ivec3 v){return get(v.x,v.y,v.z);}
        void set(int x,int y,int z,BlockState b){
            if(in_bounds(x,y,z))map[x/16][z/16].set(x%16,y,z%16,b);
        }
        void set(glm::ivec3 v,BlockState b){set(v.x,v.y,v.z,b);}

    };

}

#endif //GLMINECRAFT3_BLOCK_H
