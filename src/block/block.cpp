//
// Created by Jack Armstrong on 11/15/19.
//

#include <boost/array.hpp>
#include "block.h"
#include "blocks.h"

namespace block {

    void render_full_block(gl::mesh_data *md, block_context ctx, client::quad_texture_descr xmi, client::quad_texture_descr xpl,
                           client::quad_texture_descr ymi, client::quad_texture_descr ypl, client::quad_texture_descr zmi,
                           client::quad_texture_descr zpl) {
        client::quad_descr qd;
        if (ctx.xmi == 0) {
            qd.xmi();
            qd.texture = xmi;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.xpl == 0) {
            qd.xpl();
            qd.texture = xpl;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }

        if (ctx.ymi == 0) {
            qd.ymi();
            qd.texture = ymi;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.ypl == 0) {
            qd.ypl();
            qd.texture = ypl;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }

        if (ctx.zmi == 0) {
            qd.zmi();
            qd.texture = zmi;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.zpl == 0) {
            qd.zpl();
            qd.texture = zpl;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
    }

    void block::render(gl::mesh_data *md, block_context ctx) const noexcept {
        printf("le wot, %i\n", ctx.self);
        std::raise(11);
    }

    chunk::chunk() {
        palette.push_back(0);
        palette_set.insert(0);
    }

    bool chunk::in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < 16 && y < 256 && z < 16;
    }

    bool chunk::in_bounds(glm::ivec3 v) const { return in_bounds(v.x, v.y, v.z); }

    block_state chunk::get(int x, int y, int z) const {
        if (in_bounds(x, y, z))return palette[data[x][y][z]];
        return 0;
    }

    block_state chunk::get(glm::ivec3 v) const { return get(v.x, v.y, v.z); }

    void chunk::set(int x, int y, int z, block_state b) {
        if (in_bounds(x, y, z)) {
            if (palette_set.count(b) == 0) {
                palette.push_back(b);
                palette_set.insert(b);
                data[x][y][z] = palette.size() - 1;
            } else {
                data[x][y][z] = std::distance(palette.begin(), std::find(palette.begin(), palette.end(), b));
            }
        }
    }

    void chunk::set(glm::ivec3 v, block_state b) { set(v.x, v.y, v.z, b); }

    void chunk::serialize(int section_y, boost::array<long, 4096> &array) {
        int i=0;
        for(int x=0;x<16;x++){
            for(int y=0;y<16;y++){
                for(int z=0;z<16;z++){
                    array[i]=palette[data[x][y+section_y*16][z]];
                    i++;
                }
            }
        }
    }

    void chunk::read(int section_y, boost::array<long, 4096> &array) {
        int i=0;
        for(int x=0;x<16;x++){
            for(int y=0;y<16;y++){
                for(int z=0;z<16;z++){
                    set(x,y+section_y*16,z,array[i]);
                    i++;
                }
            }
        }
    }

}