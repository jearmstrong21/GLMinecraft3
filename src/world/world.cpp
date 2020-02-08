//
// Created by Jack Armstrong on 11/16/19.
//

#include <memory>
#include "world.h"

namespace block {

    void world_op::save(const nbt::nbt_compound_ptr &tag) {
        tag->value["pos"] = nbt::nbt_list::make(
                {nbt::nbt_int::make(pos.x), nbt::nbt_int::make(pos.y), nbt::nbt_int::make(pos.z)});
        tag->value["nstate"] = nbt::nbt_long::make(nstate);
    }

    void world_op::load(const nbt::nbt_compound_ptr &tag) {
        pos = {tag->value["pos"]->list_ref()[0]->as_int(), tag->value["pos"]->list_ref()[1]->as_int(),
               tag->value["pos"]->list_ref()[2]->as_int()};
        nstate = tag->value["nstate"]->as_long();
    }

    bool world::in_bounds(int x, int y, int z) const {
        return x >= 0 && y >= 0 && z >= 0 && x < WORLD_SIZE * 16 && y < 256 && z < WORLD_SIZE * 16;
    }

    bool world::in_bounds(glm::ivec3 v) const { return in_bounds(v.x, v.y, v.z); };

    block_state world::get(int x, int y, int z) const {
        if (in_bounds(x, y, z))return map[x / 16][z / 16]->get(x % 16, y, z % 16);
        return 0;
    }

    block_state world::get(glm::ivec3 v) const { return get(v.x, v.y, v.z); }

    world_op world::set(int x, int y, int z, block_state b) {
        if (in_bounds(x, y, z)) {
            map[x / 16][z / 16]->set(x % 16, y, z % 16, b);
            return {true, {x, y, z}, b};
        }
        return {false};
    }

    world_op world::set(glm::ivec3 v, block_state b) { return set(v.x, v.y, v.z, b); }

    block_context world::get_block_context(glm::ivec3 p) {
        return block_context{get(p.x - 1, p.y, p.z), get(p.x + 1, p.y, p.z), get(p.x, p.y - 1, p.z),
                             get(p.x, p.y + 1, p.z), get(p.x, p.y, p.z - 1), get(p.x, p.y, p.z + 1), get(p), this, p};
    }

    world::world() {
        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
                map[x][z] = std::make_shared<chunk>();
            }
        }
    }

    void world::generate_world() {
        world_generator generator;
        generator.generate_world(this);
    }

    void world::apply(world_op op) {
        if (in_bounds(op.pos)) {
            map[op.pos.x / 16][op.pos.z / 16]->set(op.pos.x % 16, op.pos.y, op.pos.z % 16, op.nstate);
        }
    }
}