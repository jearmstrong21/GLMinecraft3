//
// Created by Jack Armstrong on 11/15/19.
//

#include <boost/array.hpp>
#include "block.h"
#include "blocks.h"
#include "world/world.h"

namespace block {

    float calculate_ao(world *world, glm::ivec3 block_pos) {
        int x = 0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    if (world->get(block_pos.x - i, block_pos.y - j, block_pos.z - k) == 0)x++;
                }
            }
        }
        return x / 8.0F;
    }

    void render_full_block(gl::mesh_data *md, block_context ctx, client::quad_texture_descr xmi,
                           client::quad_texture_descr xpl,
                           client::quad_texture_descr ymi, client::quad_texture_descr ypl,
                           client::quad_texture_descr zmi,
                           client::quad_texture_descr zpl) {
        client::quad_descr qd;
        glm::ivec3 v_000 = ctx.pos;
        glm::ivec3 v_100 = ctx.pos + glm::ivec3{1, 0, 0};
        glm::ivec3 v_010 = ctx.pos + glm::ivec3{0, 1, 0};
        glm::ivec3 v_001 = ctx.pos + glm::ivec3{0, 0, 1};
        glm::ivec3 v_011 = ctx.pos + glm::ivec3{0, 1, 1};
        glm::ivec3 v_101 = ctx.pos + glm::ivec3{1, 0, 1};
        glm::ivec3 v_110 = ctx.pos + glm::ivec3{1, 1, 0};
        glm::ivec3 v_111 = ctx.pos + glm::ivec3{1, 1, 1};
        float _000 = calculate_ao(ctx.world, v_000);
        float _100 = calculate_ao(ctx.world, v_100);
        float _010 = calculate_ao(ctx.world, v_010);
        float _001 = calculate_ao(ctx.world, v_001);
        float _011 = calculate_ao(ctx.world, v_011);
        float _101 = calculate_ao(ctx.world, v_101);
        float _110 = calculate_ao(ctx.world, v_110);
        float _111 = calculate_ao(ctx.world, v_111);
        if (ctx.xmi == 0) {
            qd.xmi();
            qd.texture = xmi;
            qd.ao[0] = _000;
            qd.ao[1] = _001;
            qd.ao[2] = _010;
            qd.ao[3] = _011;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.xpl == 0) {
            qd.xpl();
            qd.texture = xpl;
            qd.ao[0] = _100;
            qd.ao[1] = _101;
            qd.ao[2] = _110;
            qd.ao[3] = _111;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }

        if (ctx.ymi == 0) {
            qd.ymi();
            qd.texture = ymi;
            qd.ao[0] = _000;
            qd.ao[1] = _100;
            qd.ao[2] = _001;
            qd.ao[3] = _101;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.ypl == 0) {
            qd.ypl();
            qd.texture = ypl;
            qd.ao[0] = _010;
            qd.ao[1] = _110;
            qd.ao[2] = _011;
            qd.ao[3] = _111;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }

        if (ctx.zmi == 0) {
            qd.zmi();
            qd.texture = zmi;
            qd.ao[0] = _000;
            qd.ao[1] = _100;
            qd.ao[2] = _010;
            qd.ao[3] = _110;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.zpl == 0) {
            qd.zpl();
            qd.texture = zpl;
            qd.ao[0] = _001;
            qd.ao[1] = _101;
            qd.ao[2] = _011;
            qd.ao[3] = _111;
            client::mesh_quad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
    }

    void block::render(gl::mesh_data *md, block_context ctx) const noexcept {
        printf("le wot, %li\n", ctx.self);
        std::exit(11);
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
        int i = 0;
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    array[i] = palette[data[x][y + section_y * 16][z]];
                    i++;
                }
            }
        }
    }

    void chunk::read(int section_y, boost::array<long, 4096> &array) {
        int i = 0;
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    set(x, y + section_y * 16, z, array[i]);
                    i++;
                }
            }
        }
    }

}