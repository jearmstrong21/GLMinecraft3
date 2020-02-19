//
// Created by Jack Armstrong on 2/16/20.
//

#include "item_block.h"
#include <utility>
#include "server/game_room.h"

namespace item {

    item_block::item_block(item_properties properties, block::block_state state) : item(std::move(properties)),
                                                                                   state(state) {

    }

    void item_block::use(const item_use_context &ctx) {
        if (ctx.stack->is_empty())return;
        if (ctx.source->type_id == ENTITY_ID_PLAYER) {
            auto player = (entity::entity_player *) ctx.source;
            if (server::game_room::instance->place_block(player->intersection.prev, state)) {
                ctx.stack->count--;
            }
        }
    }

    item_stack item_block::make() {
        item_stack s;
        s.item_type_id = properties.item_type_id;
        s.count = 1;
        return s;
    }

    client::item_texture_descr item_block::render(item_stack stack) {
        client::item_texture_descr tex = render_no_count(stack);
        tex.count = stack.count;
        return tex;
    }

    item_block_default::item_block_default(item_properties properties, block::block_state state,
                                           client::item_texture_descr tex) : item_block(std::move(properties), state),
                                                                             tex(std::move(tex)) {

    }

    client::item_texture_descr item_block_default::render_no_count(item_stack stack) {
        return tex;
    }
}