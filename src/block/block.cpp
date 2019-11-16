//
// Created by Jack Armstrong on 11/15/19.
//

#include "block.h"

namespace block {

    void renderFullBlock(gl::MeshData *md, BlockContext ctx, client::QuadTextureDescr xmi, client::QuadTextureDescr xpl,
                         client::QuadTextureDescr ymi, client::QuadTextureDescr ypl, client::QuadTextureDescr zmi,
                         client::QuadTextureDescr zpl) {
        client::QuadDescr qd;
        if (ctx.xmi == 0) {
            qd.xmi();
            qd.texture = xmi;
            client::meshQuad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.xpl == 0) {
            qd.xpl();
            qd.texture = xpl;
            client::meshQuad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }

        if (ctx.ymi == 0) {
            qd.ymi();
            qd.texture = ymi;
            client::meshQuad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.ypl == 0) {
            qd.ypl();
            qd.texture = ypl;
            client::meshQuad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }

        if (ctx.zmi == 0) {
            qd.zmi();
            qd.texture = zmi;
            client::meshQuad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
        if (ctx.zpl == 0) {
            qd.zpl();
            qd.texture = zpl;
            client::meshQuad(md, qd, ctx.pos.x, ctx.pos.y, ctx.pos.z);
        }
    }

    void Block::render(gl::MeshData *md, BlockContext ctx) const noexcept {
        printf("le wot, %i\n", ctx.self);
        std::raise(11);
    }

    Chunk::Chunk() {
        palette.push_back(0);
        paletteSet.insert(0);
    }

    bool Chunk::in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < 16 && y < 256 && z < 16;
    }

    bool Chunk::in_bounds(glm::ivec3 v) const { return in_bounds(v.x, v.y, v.z); }

    BlockState Chunk::get(int x, int y, int z) const {
        if (in_bounds(x, y, z))return palette[data[x][y][z]];
        return 0;
    }

    BlockState Chunk::get(glm::ivec3 v) const { return get(v.x, v.y, v.z); }

    void Chunk::set(int x, int y, int z, BlockState b) {
        if (in_bounds(x, y, z)) {
//                printf("SET %i,%i,%i: %i\n",x,y,z,b);
            if (paletteSet.count(b) == 0) {
                palette.push_back(b);
                paletteSet.insert(b);
                data[x][y][z] = palette.size() - 1;
            } else {
                data[x][y][z] = std::distance(palette.begin(), std::find(palette.begin(), palette.end(), b));
            }
        }
    }

    void Chunk::set(glm::ivec3 v, BlockState b) { set(v.x, v.y, v.z, b); }

}